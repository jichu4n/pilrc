Name: pilrc
Summary: Palm OS resources compiler
Version: 2.9
Release: 1
Source: %{name}-%{version}.tar.gz
Group: Development/Palm
URL: http://www.ardiri.com/index.php?redir=palm&cat=pilrc
BuildRoot: /var/tmp/%{name}-buildroot
Copyright: GPL
Prefix: /usr
Packager: Martin Brachtl <brachtl@redgrep.cz>

%description
PilRC (which is an abbreviation of PILot Resource Compiler) takes a text
description of forms, menus, and other instances of Palm OS resources and
compiles them into the binary form used on Palm OS.

%package -n pilrcui
Summary: A GNOME application for previewing Palm OS resources
Group: Development/Palm

%description -n pilrcui
PilRCUI is a GNOME application which reads the same source files as PilRC,
a Palm OS resource compiler, and displays a preview window showing the
resources which are to be generated.

%prep
rm -rf $RPM_BUILD_ROOT

%setup -q

%build
./configure
make
cd fonts;unzip *.zip;make

%install
mkdir -p $RPM_BUILD_ROOT/usr/bin
install -m 0755 pilrc $RPM_BUILD_ROOT/usr/bin/pilrc
install -m 0755 pilrcui $RPM_BUILD_ROOT/usr/bin/pilrcui
install -m 0755 fonts/pilfont $RPM_BUILD_ROOT/usr/bin/pilfont

%clean
rm -rf $RPM_BUILD_ROOT 

%files
%doc LICENSE.txt
%doc README.txt
%doc doc
%doc example
%doc fonts/COPYING
%doc fonts/README
/usr/bin/pilrc
/usr/bin/pilfont

%files -n pilrcui
/usr/bin/pilrcui
%doc LICENSE.txt
%doc README.txt
