Name: pilrc
Summary: Pilot Resources Compiler
Version: 2.8p6
Release: 1
Source: %{name}-%{version}.tar.gz
Group: Development/Palm
URL: http://www.ardiri.com/
BuildRoot: /var/tmp/%{name}-buildroot
Copyright: GPL
Prefix: /usr
Packager: Martin Brachtl <brachtl@redgrep.cz>

%description
Palm resources compiler

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
/usr/bin/pilrcui
/usr/bin/pilfont

