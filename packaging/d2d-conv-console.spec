## Basic Descriptions of this package
Name:       d2d-conv-console
Summary:    D2D Convergence Console Service
Version:    0.1
Release:    1
Group:      Framework/system
License:    Apache License, Version 2.0
Source0:    %{name}-%{version}.tar.gz
Source1:    d2d-conv-console.service

# Required packages
# Pkgconfig tool helps to find libraries that have already been installed
BuildRequires:  cmake
BuildRequires:  libattr-devel
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(dlog)

## Description string that this package's human users can understand
%description
D2D Convergence Console Service


## Preprocess script
%prep
# setup: to unpack the original sources / -q: quiet
# patch: to apply patches to the original sources
%setup -q

## Build script
%build
# 'cmake' does all of setting the Makefile, then 'make' builds the binary.
cmake . -DCMAKE_INSTALL_PREFIX=/usr
make %{?jobs:-j%jobs}

## Install script
%install
# make_install: equivalent to... make install DESTDIR="%(?buildroot)"
%make_install

# install license file
mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}

# install systemd service
mkdir -p %{buildroot}%{_libdir}/systemd/system/graphical.target.wants
install -m 0644 %SOURCE1 %{buildroot}%{_libdir}/systemd/system/
ln -s ../d2d-conv-console.service %{buildroot}%{_libdir}/systemd/system/graphical.target.wants/d2d-conv-console.service

## Postprocess script
%post

## Binary Package: File list
%files
%manifest d2d-conv-console.manifest
%{_bindir}/d2d-conv-console-svc
%{_libdir}/systemd/system/d2d-conv-console.service
%{_libdir}/systemd/system/graphical.target.wants/d2d-conv-console.service
/usr/share/license/%{name}
