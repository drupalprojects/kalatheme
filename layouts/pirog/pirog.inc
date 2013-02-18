<?php
/**
 * @file
 * Plugin definition.
 */

$plugin = array(
  'title' => t('Pirog'),
  'icon' => 'piroggreen.png',
  'category' => t('Kalatheme'),
  'theme' => 'pirog',
  'css' => 'pirog.css',
  'regions' => array(
    'header' => t('Header'),
    'feature' => t('Featured'),
    'main' => t('Main'),
    'second' => t('Second'),
    'footer1' => t('Row 1: First Column'),
    'footer2' => t('Row 1: Second Column'),
    'footer3' => t('Row 1: Third Column'),
  ),
);
