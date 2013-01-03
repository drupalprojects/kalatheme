<?php

/**
 * @file
 * PHP CLI script to compile Bootstrap's .less files to separate .css files
 *
 * Requires system to have the lessc command available
 *
 * Usage:
 * - Copy this script to sites/all/libraries/bootstrap/less
 * - Create the directory sites/all/libraries/bootstrap/css
 * - On the command line, run: php compile-bootstrap.php
 */

/**
 * Generates output for a temporary LESS file
 *
 * @param string $file
 *   Name of the file to include in the temp file
 * @return string
 *   Contents of the temp file
 */
function generate_less($file) {
  return <<< _END
    @import "variables.less";
    @import "mixins.less";
    @import "$file";
_END;
}

$excluded_files = array(
  '.', '..', 'variables.less', 'mixins.less', 'responsive-1200px-min.less',
  'responsive-767px-max.less', 'responsive-768px-979px.less', 'responsive.less',
  'responsive-navbar.less', 'responsive-utilities.less', 'compile-bootstrap.php',
  'bootstrap.less', 'temp.less',
);

$bootstrap_directory = dirname(dirname(__FILE__));
$css_directory = $bootstrap_directory . '/css';
$less_directory = $bootstrap_directory . '/less';

if (!file_exists($css_directory)) {
  die('Please create the directory "css" next to the less directory.');
}

$temp_less_file = $less_directory . '/temp.less';

$bootstrap_files = scandir($less_directory);

foreach ($bootstrap_files as $file) {
  if (in_array($file, $excluded_files) || is_dir($file) || (strpos($file, '.less') === FALSE)) {
    continue;
  }
  file_put_contents($temp_less_file, generate_less($file));
  $css_file = str_replace('less', 'css', $file);
  echo $file . '...' . shell_exec('lessc "' . $temp_less_file . '" "' . $css_directory . '/' . $css_file . '"') . "\n";
}

unlink($temp_less_file);

echo 'Responsive... ' . shell_exec('lessc "' . $less_directory . '/responsive.less" "' . $css_directory . '/responsive.css"') . "\n";

echo "\n\nBootstrap compilation complete!\n";
