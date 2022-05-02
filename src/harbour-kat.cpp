/*
  Copyright (C) 2016 Petr Vytovtov
  Contact: Petr Vytovtov <osanwe@protonmail.ch>
  All rights reserved.

  This file is part of Kat.

  Kat is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Kat is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Kat.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QStandardPaths>
#include <QQmlContext>
#include <QQuickView>
#include <QScopedPointer>
#include <QtGlobal>

#include <stdio.h>
#include <stdlib.h>
#include </home/okabe2011/SailfishOS/mersdk/targets/SailfishOS-3.4.0.24-armv7hl.default/usr/include/sailfishapp/sailfishapp.h>

#include "filesaver.h"
#include "mediaplayerwrapper.h"
#include "settingswrapper.h"
#include "vksdk/src/vksdk.h"

/*void myMessageOutputt(QtMsgType type, const char *msg)
 {

   // QByteArray localMsg = msg.toLocal8Bit();
    QFile fileOut("/home/defaultuser/log.txt");
        if(fileOut.open(QIODevice::Append | QIODevice::Text))
        {
            fileOut.write(msg);
            fileOut.flush();
            fileOut.close();
    }

 }*/


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type);
    Q_UNUSED(context);
    Q_UNUSED(msg);
    QByteArray localMsg = msg.toLocal8Bit();
    //switch (type) {
    /*case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }*/

    QFile fileOut("/home/nemo/log.txt");
        if(fileOut.open(QIODevice::Append | QIODevice::Text))
        {
            fileOut.write(localMsg);
            fileOut.flush();
            fileOut.close();
    }
}


int main(int argc, char *argv[]) {
    QScopedPointer<QGuiApplication> application(SailfishApp::application(argc, argv));
    application->setOrganizationName(QStringLiteral("org.ilyavysotsky"));
    application->setApplicationName(QStringLiteral("kat"));
        qInstallMessageHandler(myMessageOutput);
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    QScopedPointer<FileSaver> fileSaver(new FileSaver(view.data()));
    view->rootContext()->setContextProperty("fileSaver", fileSaver.data());

    QScopedPointer<MediaPlayerWrapper> player(new MediaPlayerWrapper(view.data()));
    view->rootContext()->setContextProperty("player", player.data());

    QScopedPointer<SettingsWrapper> settings(new SettingsWrapper(view.data()));
    view->rootContext()->setContextProperty("settings", settings.data());

    QScopedPointer<VkSDK> vksdk(new VkSDK(view.data()));
    view->rootContext()->setContextProperty("vksdk", vksdk.data());

    view->setSource(SailfishApp::pathTo("qml/harbour-kat.qml"));
    view->show();

    return application->exec();
}

