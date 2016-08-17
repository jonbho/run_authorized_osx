run_authorized_osx -- Ask for authorization and run command in OS X
===================================================================

BUILDING
--------
* `git clone https://github.com/jonbho/run_authorized_osx.git`
* `make`

It just invokes `gcc` to build, including the `Security` OS X framework to access the required APIs.

USAGE
-----
* Invoke as `run_authorized_osx whatever command you want`, where `whatever command you want` is what you want to run authorized.
* This will ask for admin rights to run the command `whatever command you want`.
* If the user does not enter the right password, the command will not be run.
* Exit status is -1 if there is any issue obtaining authorization from the user, or whatever the command returns in any other case.
* The first parameter given is taken as the command to be executed, the rest are passed in as arguments to that command.
* The $PATH variable is **not** checked to run the command, you need to provide the full path to the executable.

IMPORTANT NOTE
--------------
Unfortunately, the main API used by this little utility, `AuthorizationExecuteWithPrivileges`, is **deprecated**
by Apple :( Thus, it will stop working at some point in the (near) future. The suppoted alternative, SMJobBless, is
a lot less flexible, can only be used inside an app bundle, etc...

FUTURE EVOLUTION
----------------
It's difficult to makes predictions, especially about the future. But if someone adds $PATH searching, I would be very happy to merge that in...

ATTRIBUTION
-----------
Based on sample code by Michael V. O'Brien at
[https://github.com/michaelvobrien/OSXSlightlyBetterAuth](https://github.com/michaelvobrien/OSXSlightlyBetterAuth),
just shaped to be more flexibly reused from scripts, etc...

LEGAL
-----
(c) August 2016 Jon Beltran de Heredia ([jonbho](http://jonbho.net)). Licensed under the MIT license.
