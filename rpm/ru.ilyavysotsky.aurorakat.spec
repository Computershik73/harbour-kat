Name:       ru.ilyavysotsky.aurorakat
Summary:    Неофициальное приложение ВКонтакте для ОС Аврора
Version:    0.6.8
Release:    1
License:    BSD-3-Clause
URL:        https://auroraos.ru
Source0:    %{name}-%{version}.tar.bz2

Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(auroraapp)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)


%description
Неофициальное приложение ВКонтакте для ОС Аврора by @okabe2011 (Илья Высоцкий)

%prep
%autosetup

%build
%qmake5
%make_build

%install
%qmake5_install

%files
%{_bindir}/%{name}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
