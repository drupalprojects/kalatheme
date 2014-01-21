
CONTENTS OF THIS FILE
---------------------

 * Installing Kalatheme
 * Automatic Setup and Subthemes
 * Manually Installing Bootstrap
 * Creating a Subtheme
 * Key Features

INSTALLING KALATHEME
--------------------
Unlike other Drupal themes Kalatheme requires some other modules to work
properly. Please verify you have the following before proceeding with
installation. If you fail to do as Kalatheme has requested it will bug
you about it until the ending of the world.

 * Libraries 2.1+
 * Panels 3.3+
 * PHP 5.3+
 * JQuery Update 2.x (with JQuery version set to 1.7+)
 * A Bootstrap 3.0+ library (read below in Installing Bootstrap)
 *  Optional but highly recommended: Panopoly Theme.
 * Optional: A custom Bootstrap library.

The easiest way to satisfy all of these requirements is to just start with
Panopoly on Pantheon.
https://drupal.org/node/2175703

Then install Kalatheme like any other theme
http://drupal.org/documentation/install/modules-themes

Some people having troubles using the Kalacustomize plugin were helped by
Patch #5 here: https://drupal.org/node/2024441
which was rolled against Panels 3.3+41-dev

AUTOMATIC SETUP & SUBTHEMES
---------------------------
Kalatheme is meant to be the base theme that is used to build more powerful
subthemes. Subthemes inherit almost all of the propoerties of their base theme
so you can reduce theme clutter and start on the 10th floor. Here is some
documentation on creating a basic subtheme

Luckily, Kalatheme features a pretty neat subtheme generation tool that will
automatically set everything up for you and allow you to customize your
subtheme.
https://drupal.org/node/2167149


MANUALLY INSTALLING BOOTSTRAP
-----------------------------
Kalatheme doesn't do much without a Bootstrap Library so you are going to need
to shop around for one.

 * To get the standard Bootstrap library, or to customize that library:
 http://getbootstrap.com/

 * If you are looking for a free and pre-made custom version of Bootstrap:
 http://bootswatch.com/

 * If you are looking to rool with a custom version of Bootstrap try out
 http://getbootstrap.com/customize/
 https://drupal.org/node/2167149

 * If you don't mind paying for a little extra:
 http://wrapbootstrap.com/

 * You can also Google for other sources if you are feeling adventerous.
 http://www.google.com/

Kalatheme uses the Libraries API so in order to get Bootstrap working you need
to put your Bootstrap files in sites/all/libraries/CURRENT-THEME_bootstrap. For
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
