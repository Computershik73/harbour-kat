################################################################################
##
## Copyright (C) 2022 ru.ilyavysotsky
## 
## This file is part of the Неофициальное приложение ВКонтакте для ОС Аврора project.
##
## Redistribution and use in source and binary forms,
## with or without modification, are permitted provided
## that the following conditions are met:
##
## * Redistributions of source code must retain the above copyright notice,
##   this list of conditions and the following disclaimer.
## * Redistributions in binary form must reproduce the above copyright notice,
##   this list of conditions and the following disclaimer
##   in the documentation and/or other materials provided with the distribution.
## * Neither the name of the copyright holder nor the names of its contributors
##   may be used to endorse or promote products derived from this software
##   without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
## THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
## FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
## IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
## FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
## OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
## PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
## LOSS OF USE, DATA, OR PROFITS;
## OR BUSINESS INTERRUPTION)
## HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
## WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
## (INCLUDING NEGLIGENCE OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
## EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##
################################################################################

TARGET = ru.ilyavysotsky.aurorakat

PKGCONFIG += \

CONFIG += \
    auroraapp
greaterThan(QT_VERSION, 5.5) {
    CONFIG += C++11
} else {
    QMAKE_CXXFLAGS += -std=gnu++11
}

QT += multimedia core gui qml quick

include("vksdk/vksdk.pri")

HEADERS += \
    src/settingswrapper.h \
    src/mediaplayerwrapper.h \
    src/playlistmodel.h \
    src/filesaver.h \
    src/utils.h

SOURCES += \
    src/harbour-kat.cpp \
    src/settingswrapper.cpp \
    src/mediaplayerwrapper.cpp \
    src/playlistmodel.cpp \
    src/filesaver.cpp \
    src/utils.cpp


DISTFILES += \
    rpm/ru.ilyavysotsky.aurorakat.spec \
    AUTHORS.md \
    CODE_OF_CONDUCT.md \
    CONTRIBUTING.md \
    LICENSE.BSD-3-CLAUSE.md \
    README.md \
    qml/harbour-kat.qml \
    qml/cover/*.qml \
    qml/pages/*.qml \
    qml/views/*.qml \

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

TRANSLATIONS += \
    translations/ru.ilyavysotsky.aurorakat.ts \
    translations/ru.ilyavysotsky.aurorakat-ru.ts \
