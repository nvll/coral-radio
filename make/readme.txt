As it stands, this build system is designed entirely to link into embedded
SDKs like nordic's nRF51 SDK. One notable potential issue right now is that
there is no sandboxing of CFLAGS or Includes between module builds, and as
such all compilation will be done with the full parameters provided collectively
by the modules. This works great for building vs an SDK, but may cause problems
when building external libraries.

The general layout of the system is:

projects/ contains a <projectname>.mk which defines MODULES that the build requires,
as well as an OUTPUT target. The last line in a project .mk should be including the
right .mk to build the project. In the case of nRF51 sdk projects this should be make/nrf51-sdk.mk.
Each module directory listed is checked for <module>/module.mk which is then included. Modules
should add source and headers via appending to MODULE_SOURCES and MODULE_INCLUDES. If a module is
dependent on another module it can specify this by adding MODULES of their own as if they were
a project file.
