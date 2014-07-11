
CONTENTS OF THIS FILE
---------------------

 * Installing Kalatheme
 * Automatic Setup and Subthemes
 * Manually Installing Bootstrap
 * Creating a Subtheme
 * Key Features

INSTALLING KALATHEME
--------------------
## System Requirements

Unlike other Drupal themes Kalatheme requires some other modules to work properly. Please verify you have the following before proceeding with installation. If you fail to do as Kalatheme has requested it will bug you about it until the ending of the world.

1. [Libraries API 2.1+][1]
2. [Panels 3.3+][2]
3. [Views 3.x][3]
4. [JQuery Update][4] 2.x (with JQuery version set to 1.7+)
5. [PHP 5.3+][4]
6. Optional but highly recommended: [Panopoly Theme.][5]
7. Optional: A custom Bootstrap library.

The easiest way to satisfy all of these requirements is to just start with [Panopoly][6] on [Pantheon][7] as [seen here][8].

Some people also having troubles using the Kalacustomize plugin were helped by Patch #5 here:
[#2024441: Fatal error undefined function panels_plugin_get_function when doing #ajax on pane style form][9]
which was rolled against Panels 3.3+41-dev.

UPDATE: This patch is now in the latest dev version of CTools/Panels.

## Installation

Our magical quest to install Kalatheme will begin as it would for any other theme: using the [Drupal Installing Modules and Themes Guide][10]. After you have followed the above and installed your theme you should proceed to the Appearances page where you MUST ENABLE IT AND SET IT AS THE DEFAULT THEME as well.

![Enable me!][11]


AUTOMATIC SETUP & SUBTHEMES
---------------------------
Kalatheme is meant to be the base theme that is used to build more powerful
subthemes. Subthemes inherit almost all of the propoerties of their base theme
so you can reduce theme clutter and start on the 10th floor. Here is some
documentation on creating a basic subtheme

Luckily, Kalatheme features a pretty neat subtheme generation tool that will
automatically set everything up for you and allow you to customize your
subtheme. You don't even need to install a Bootstrap library, Kalatheme will pull one from Bootswatch (http://bootswatch.com/) for you!

After you enable Kalatheme it is likely you will be blinded by its immense and compelling beauty. What's that? It looks horrible? Never fear! All you need to do is set up kalatheme by going into the theme settings. You can also do this part [with drush](Installation-with-Drush).

![Kalatheme settings][12]

Currently the setup workflow is pretty stable when starting from [Pantheon][17] or [Kalabox][18]/[Kalastack][19] using the [Panopoly][6] distro.

In the settings you can easily build a Kalatheme subtheme and pair it with either default Bootstrap, [Bootswatch][20] or a [Custom Bootstrap Library][21]. Kalatheme also has limited support for [third party Bootstrap libraries][22] such as from [WrapBootstrap][23]. If you are going to try out a custom or third party library it may be helpful to get your read on [here](Using-custom-Bootstrap-Libraries) before attempting. You may also optionally pull [Font Awesome][25] into your subtheme.

If you want Kalatheme to install and enable these things for you automatically please [make sure your webserver is configured properly](Configuring-Server-for-Automatic-Kalatheme-installation). If not Kalatheme will simply give you an archive of goodies that you will then need to [do something with](Manual-Installation).

If you have no idea what any of the above is, you really just need to fill out the settings form like below--pick a bootstrap library, add Font Awesome if you like, name your subtheme, hit Build and enable subtheme, and voila!

Note: If you are using Pantheon, kalatheme will ask you to switch your connection mode to SFTP to complete the subtheme generation. You should take heed! If you are already in SFTP, this friendly message will still appear, but you don't need to do anything.

![Settings screenshot][28]
![Settings screenshot][29]
![Settings screenshot][30]

## Victory

After enabling your new subtheme you will notice that things are styled again. At this point it is best to proceed to [Using Kalatheme and Best Practices](Using-Kalatheme-and-Best-Practices).

![Default Kalatheme][31]

[1]: https://drupal.org/node/1938254
[2]: https://drupal.org/project/panels
[3]: https://drupal.org/project/views
[4]: https://drupal.org/project/jquery_update
[5]: https://drupal.org/project/panopoly_theme
[6]: https://drupal.org/project/panopoly
[7]: https://dashboard.getpantheon.com/products/panopoly/spinup
[8]: http://www.youtube.com/watch?v=z3y1cdc22cU
[9]: https://www.drupal.org/node/2024441 "Status: Closed (fixed)"
[10]: https://drupal.org/documentation/install/modules-themes
[11]: https://www.drupal.org/files/enabletheme.png
[12]: https://raw.githubusercontent.com/wiki/katypool/kalatheme/settings_setup.PNG
[13]: http://getbootstrap.com/
[15]: https://www.drupal.org/files/setup_0.png
[17]: http://www.getpantheon.com
[18]: http://www.kalamuna.com/products/kalabox/
[19]: https://github.com/kalamuna/kalastack
[20]: http://bootswatch.com/
[21]: http://getbootstrap.com/customize/
[22]: https://www.google.com/#q=twitter+bootstrap+3+themes&amp;safe=off
[23]: https://wrapbootstrap.com/
[25]: http://fontawesome.io/
[28]: https://raw.githubusercontent.com/wiki/katypool/kalatheme/subtheme_generator1.PNG
[29]: https://raw.githubusercontent.com/wiki/katypool/kalatheme/subtheme_generator2.PNG
[30]: https://raw.githubusercontent.com/wiki/katypool/kalatheme/subtheme_generator3.PNG
[31]: https://www.drupal.org/files/Screen%20Shot%202014-01-04%20at%204.30.20%20PM.png


MANUALLY INSTALLING BOOTSTRAP
-----------------------------
Kalatheme no longer requires an installed Bootstrap library to work. Our subtheme generator does that for you. However, if you are looking to create a custom bootstrap library, here are some options:

 * To get the standard Bootstrap library, or to customize that library:
 http://getbootstrap.com/

 * If you are looking to rool with a custom version of Bootstrap try out
 http://getbootstrap.com/customize/
 https://drupal.org/node/2167149

 * If you don't mind paying for a little extra:
 http://wrapbootstrap.com/

 * You can also Google for other sources if you are feeling adventerous.
 http://www.google.com/

If you choose to manually install a custom Bootstrap library, put your Bootstrap files in sites/all/libraries/CURRENT-THEME_bootstrap. For
example, if you have a Kalatheme subtheme enabled called mytheme, you'd put
Bootstrap's files in sites/all/libraries/mytheme_bootstrap. If you have
Kalatheme set as your default theme, you'd use
sites/all/libraries/kalatheme_bootstrap.
This is so you can have differently customized installations of Bootstrap for
different themes.

Custom Bootstrap libraries can use a non-standard files scheme so you need to
make sure that your bootstrap directory looks like the following folders and
files.

  /CURRENT-THEME_bootstrap
  /CURRENT-THEME_bootstrap/css
  /CURRENT-THEME_bootstrap/css/bootstrap.css
  /CURRENT-THEME_bootstrap/css/bootstrap.min.css
  /CURRENT-THEME_bootstrap/fonts/
  /CURRENT-THEME_bootstrap/js/
  /CURRENT-THEME_bootstrap/js/bootstrap.js
  /CURRENT-THEME_bootstrap/js/bootstrap.min.js

IMPORTANT: The only actual requirement here is that either css/bootstrap.css or
css/bootstrap.min.css exist and that they both have some sort of version
information at the top like this:

  /*!
   * Bootstrap v3.0.0
   *
   * Copyright 2013 Twitter, Inc
   * Licensed under the Apache License v2.0
   * http://www.apache.org/licenses/LICENSE-2.0

Most themes have this by default and you can use the above as a basis. It is
also worth noting that while you only need boostrap.(min).css for this to "work"
you will likely be disappointed if you don't have the JS and font files as well.

If you have more files than what is listed above we recommend putting these
files in a KalaSUBtheme.

You also do not need to have the minified files to get this to work but they are
highly recommended for better performance.

KEY FEATURES
------------
 * Settings
 On the settings page for Kalatheme you can configure how you want the style
 plugin to work.
 https://drupal.org/node/2167213

 * Style Plugin
 When you choose to "Customize this page" using the Panels In-Place Editor you
 gain access to a bunch of customization tools provided by Kalatheme. Select the
 paintbrush on the panels pane or region you want to edit, choose
 "Kalacustomize" and hit next.
 https://drupal.org/node/2167217

 * Views Grid
 Any content pane view that is made with the grid display and that has an
 amount of columns that can evenly divide the amount of columns in your
 responsive grid (12 by default) will be automatically responsive. You can
 also implement a custom size grid.
 https://drupal.org/node/2167219

 * Responsive Menu and Toggling
 The "main-menu" menu will automatically dropdown for subitems. It will also
 automatically "responsify" on tablet and phone. You can also choose what
 device you want each pane to show up on.
 https://drupal.org/node/2167215

 * One Region Theme Nirvana with Panels Layouts
 https://drupal.org/node/2167223
 https://drupal.org/node/2167225

 * SASS and COMPASS Support
 https://drupal.org/node/2167227
