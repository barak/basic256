/** Copyright (C) 2016, Florin Oprea <florinoprea.contact@gmail.com>
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **/


#include "BasicDownloader.h"

#include <QTimer>

BasicDownloader::BasicDownloader(Error *e) :
    QObject()
{
    error = e;
    m_data = NULL;
    netreply = NULL;
    reply = true;
    cancel = false;
    inprogress = false;
    connect(&netmanager, SIGNAL (finished(QNetworkReply*)),this, SLOT (fileDownloaded(QNetworkReply*)));
}

BasicDownloader::~BasicDownloader() {
    stop();
}

void BasicDownloader::download(QUrl url){
    reply = false;
    inprogress = true;
    // Drop the previous download's bytes up front. fileDownloaded() only
    // assigns m_data on success, so without this a failed fetch would hand the
    // caller whatever the last successful one returned -- which now matters,
    // because SOUND/SOUNDPLAY of a URL on WASM turns the result straight into a
    // playable sound resource.
    m_data.clear();
    QNetworkRequest request(url);
//compile with older Ot (for Linux users)
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                          QNetworkRequest::NoLessSafeRedirectPolicy);
#endif
    request.setHeader(QNetworkRequest::UserAgentHeader, "App/1.0");
    if(!cancel){
        netreply = netmanager.get(request);
        connect(this, SIGNAL(cancelDownload()), netreply, SLOT(abort()));
        if(cancel) emit(cancelDownload());
    }else{
        cancel=false;
        inprogress = false;
    }
}

void BasicDownloader::fileDownloaded(QNetworkReply* ) {
    //qDebug() << "BasicDownloader fileDownloaded() attr:" << netreply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString() << "err:" << netreply->error();
    if(netreply->error() != QNetworkReply::NoError) {
        error->q(ERROR_DOWNLOAD, netreply->errorString());
    }else if(netreply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() >= 300) {
        error->q(ERROR_DOWNLOAD, netreply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
    }else{
        m_data = netreply->readAll();
    }
    netreply->deleteLater();
    reply = true;
    emit(done());
    inprogress = false;
}

QByteArray BasicDownloader::data() const {
    if(!reply){
        // Bounded wait. A request that never completes -- offline, or a URL the
        // browser refuses to serve and reports nothing about -- would otherwise
        // park the calling thread for the rest of the run. Callers already read
        // an empty result as a failed load.
        QEventLoop loop;
        QTimer timer;
        timer.setSingleShot(true);
        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        connect(this, SIGNAL(done()), &loop, SLOT(quit()));
        timer.start(30000);
        loop.exec();
    }
    return m_data;
}

void BasicDownloader::stop() {
    if(inprogress){
        cancel=true;
        emit(cancelDownload());
    }
}
