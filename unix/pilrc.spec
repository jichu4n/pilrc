Name: pilrc
Version: 3.0alpha
Release: 1
Summary: Palm OS resource compiler
License: GPL
URL: http://www.ardiri.com/index.php?redir=palm&cat=pilrc
Group: Development/Palm OS
Source: %{name}-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot
BuildRequires: unzip
Prefix: %{_exec_prefix}

%description
PilRC (which is an abbreviation of PILot Resource Compiler) takes a text
description of forms, menus, and other instances of Palm OS resources and
compiles them into the binary form used on Palm OS.

%package -n pilrcui
Summary: A GNOME application for previewing Palm OS resources
Group: Development/Palm OS
%description -n pilrcui
PilRCUI is a GNOME application which reads the same source files as PilRC,
a Palm OS resource compiler, and displays a preview window showing the
resources which are to be generated.

%prep
%setup -q

%build
unix/configure --bindir=%{_bindir} --enable-pilrcui
make

(cd fonts
 unzip *.zip
 make
 cp README README-pilfont.txt)

%install
[ ${RPM_BUILD_ROOT:-/} != / ] && rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install
install -m 0755 fonts/pilfont $RPM_BUILD_ROOT%{_bindir}/pilfont

%clean
[ ${RPM_BUILD_ROOT:-/} != / ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
%{_bindir}/pilrc
%{_bindir}/pilfont
%doc LICENSE.txt
%doc README.txt
%doc doc/*.html doc/images
%doc fonts/README-pilfont.txt
%doc example

%files -n pilrcui
%defattr(-, root, root)
%{_bindir}/pilrcui
%doc LICENSE.txt
%doc README.txt
