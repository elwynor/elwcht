<pre>
    _______        __    __                          
   / ___/ /  ___ _/ /_  / /  ___  __ _____  ___ ____ 
  / /__/ _ \/ _ `/ __/ / /__/ _ \/ // / _ \/ _ `/ -_)
  \___/_//_/\_,_/\__/ /____/\___/\_,_/_//_/\_, /\__/ 
                                          /___/      
                                        
                    version 1.1
       Copyright (c) 2024 Elwynor Technologies
</pre>

## WHAT IS CHAT LOUNGE?

 Chat Lounge is a simple teleconference which allows people to use aliases 
 for total anonymity. It includes generic action ("ga" command) support.
 
## USAGE

 Enter the module, provide an alias, and chat!  Everything you type
 goes out to everyone in the Chat Lounge.   
 
## COMMANDS
 
 Type your message, and it goes out to everyone present.  Just hitting enter
 will display a list of the aliases in the Chat Lounge.  Typing help or /h
 will provide a short help text.

 To send an action you can use the "ga" command, for example:
<pre>   ga is laughing out loud. </pre>

 This would send the following message to all other users in the Chat Lounge:
<pre>   <alias> is laughing out loud. </pre>

 SysOps can use a "scan" command to learn the identity of users, if necessary.
  
## INSTALLATION AND CONFIGURATION
 
 Simply unzip the archive to your BBS directory, add the module to your menu,
 configure the MSG file to your liking, and start the BBS! It's that easy!

## MODULE HISTORY
 
 Chat Lounge was written and developed by Mike Polzin of WilderLand Software.
 Elwynor Technologies acquired the module in 2004, and ported it to Worldgroup
 v3.2 in February 2006. In September 2024, it was ported to Major BBS V10.
 
## LICENSE

 This project is licensed under the AGPL v3. Additional terms apply to 
 contributions and derivative projects. Please see the LICENSE file for 
 more details.

## CONTRIBUTING

 We welcome contributions from the community. By contributing, you agree to the
 terms outlined in the CONTRIBUTING file.

## CREATING A FORK

 If you create an entirely new project based on this work, it must be licensed 
 under the AGPL v3, assign all right, title, and interest, including all 
 copyrights, in and to your fork to Rick Hadsall and Elwynor Technologies, and 
 you must include the additional terms from the LICENSE file in your project's 
 LICENSE file.

## COMPILATION

 This is a Worldgroup 3.2 / Major BBS v10 module. It's compiled using Borland
 C/C++ 5.0 for Worldgroup 3.2. If you have a working Worldgroup 3.2 development
 kit, a simple "make -f ELWCHT" should do it! For Major BBS v10, import this
 project folder in the isv/ subtree of Visual Studio 2022, right click the
 project name and choose build! When ready to build for "release", ensure you
 are building for release.

## PACKING UP

 The DIST folder includes all of the items that should be packaged up in a 
 ELWCHT.ZIP. When unzipped in a Worldgroup 3.2 or Major BBS V10 installation 
 folder, it "installs" the module.
