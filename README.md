# MemberStat

[![N|Solid](https://cdn.studio-startingames.com/startingames/images/STAR-LOGOTEXT-software.png)](https://www.studio-startingames.com/)

MemberStat is an application distributed by Startingames allowing the management/archiving of members & events.
This application is a simple solution for any small- and medium-sized organisations (Business, Non-profit organization...).

# Main Features

  - Manage Members
  - Manage Events
  - Users Management System
  - Local Mode (SQLite) or Server Mode (MySQL type)

# Licenses

This software is under [Attribution-NonCommercial-NoDerivatives 4.0 International](https://creativecommons.org/licenses/by-nc-nd/4.0/)
See license.txt & license-startingames.txt for more details.

# Security Note

To improve the software security, encryption functions has not commited !
Theses functions are located in a secret.h file. This file is the propriety of Startingames and is missing on repository.
All sources code can be build without this file without change. Preprocessor will automaticaly replace theses functions by "bypass function" that make no encryption.

# Compatibility

Like explained previously, Startingames add files at the compilation step. For this reason, MemberStat compiled from Sources will be incompatible with MemberStat distributed by Startingames.
