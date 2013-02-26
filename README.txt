
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

 * You can also Google for other sources if you are feeling adventerous.
 http://www.google.com/
 
Kalatheme uses the Libraries API so in order to get Bootstrap working you need 
to put your Bootstrap files in sites/all/libraries/bootstrap. Custom Bootstrap 
libraries can use a non-standard files scheme so you need to make sure that 
your bootstrap directory contains something like the following folders and 
files. 

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

IMPORTANT: The only actual requirement here is that bootstrap/css/bootstrap.css 
exists and has some sort of version information at the top. Most themes have this
by default and you can use the standard bootstrap library as a basis. It is also
worth nothing that while you only need boostrap.css for this to "work" you will 
likely be disappointed if you don't have the JS and responsive files as well.

If you have more files then what is listed about we recommend putting these files
in a KalaSUBtheme. 

You do not need to have the minified files to get this to work but they are highly 
recommended. 

In the case where you have bootstrap.min.css but not bootstrap.css you can simply
make a copy of bootstrap.min.css and rename it bootstrap.css. As long as the min
has version information at the top this will work. 

We are working on checking the min version if the non-min version does not exist
so the above is a workaround for now.

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
