3270 Virtual Terminal for GTK.
==============================

Created originally as part of PW3270 application.

See more details at https://softwarepublico.gov.br/social/pw3270/

Installation repositories
=========================

The latest version packaged for many linux distributions can be found in SuSE Build Service (https://build.opensuse.org/project/show/home:PerryWerneck:pw3270)

Requirements
============

 * GTK-3 (https://www.gtk.org/)
 * lib3270 (https://softwarepublico.gov.br/social/pw3270/)


Building for Linux
==================


Cross-compiling for Windows
===========================

Cross-compiling on SuSE Linux (Native or WSL)
---------------------------------------------

1. First add the MinGW Repositories for your SuSE version from:

	* 32 bits: https://build.opensuse.org/project/show/windows:mingw:win32
	* 64 bits: https://build.opensuse.org/project/show/windows:mingw:win64

2. Get lib3270 sources from git

	* git clone http://softwarepublico.gov.br/gitlab/pw3270/lib3270.git ./v3270

3. Install cross compilers

	* ./v3270/win/install-cross.sh --32 (for 32 bits)
	* ./v3270/win/install-cross.sh --64 (for 64 bits)
	* ./v3270/win/install-cross.sh --all (for 32 and 64 bits)

3. Configure build

	* ./v3270/win/win-configure.sh --32 (for 32 bits)
	* ./v3270/win/win-configure.sh --64 (for 64 bits)

