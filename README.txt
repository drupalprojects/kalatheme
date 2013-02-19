
CONTENTS OF THIS FILE
---------------------

 * Installing Kalatheme
 * Installing Bootstrap
 * Creating a Subtheme
 * Key Features

INSTALLING KALATHEME
--------------------
Kalatheme was meant to be install on top of the Panopoly ditribution. Using 
Kalatheme without Panopoly has not been tested and is not recommended. 
Furthermore, it is likely you would not be able to do much of anything without 
Panopoly.

Please download Panopoly first
http://drupal.org/project/panopoly

Then install Kalatheme like any other theme
http://drupal.org/documentation/install/modules-themes

You will notice once you install Kalatheme that it doesn't look like much and it
is giving you an error message to install a Twitter Bootstrap library. Apropos:


INSTALLING BOOTSTRAP
--------------------
Kalatheme doesn't do much without a Bootstrap Library so you are going to need 
to shop around for one. 

 * To get the standard Bootstrap library, or to customize that library:
 http://twitter.github.com/bootstrap/

 * If you are looking for a free and pre-made custom version of Bootstrap :
 http://bootswatch.com/

 * If you don't mind paying for a little extra:
 http://wrapbootstrap.com/

 * You can also Google for other source if you are feeling adventerous.
 http://www.google.com/
 
Kalatheme uses the Libraries API so in order to get Bootstrap working you need 
to put your Bootstrap files in sites/all/libraries/bootstrap. Custom Bootstrap 
libraries can use a non-standard files scheme so you need to make sure that 
your bootstrap directory contains at least the following folders and files. 

  /bootstrap
  /bootstrap/css
  /bootstrap/css/bootstrap.css
  /bootstrap/css/bootstrap.min.css
  /bootstrap/css/bootstrap-responsive.css
  /bootstrap/css/bootstrap-responsive.min.css
  /img/
  /js/
  /js/bootstrap.js
  /js/bootstrap.min.js
  
We recommend putting all other files that come with your Bootstrap library
in a KalaSUBtheme. You do not need to have the minified or responsive files to 
get this to work but they are highly recommended. 

CREATING A SUBTHEME
-------------------
Kalatheme is meant to be the base theme that is used to build more powerful 
subthemes. Subthemes inherit almost all of the propoerties of their base theme 
so you can reduce theme clutter and start on the 10th floor. Here is some 
documentation on creating a basic subtheme
http://drupal.org/node/225125

KEY FEATURES
------------
 * Settings
 On the settings page for Kalatheme you can configure how you want the style 
 plugin to work.
 
 * Style Plugin
 When you choose to "Customize this page" using the Panels In-Place Editor you 
 gain access to a bunch of customization tools provided by Kalatheme. Select the
 paintbrush on the panels pane you want to edit, choose "Kalacustomize" and hit
 next.
 
 * Views Grid 
 Any view that is made with the grid display and that has an amount of columns 
 that can divide 12 will be automatically responsive. 
 
 * Responsive Menu
 The "main-menu" menu will automatically dropdown for subitems. It will also 
 automatically "responsify" on tablet and phone.
