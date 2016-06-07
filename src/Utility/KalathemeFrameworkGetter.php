<?php

/**
 * @file
 * Contains \Drupal\kalatheme\Utility\KalathemeFrameworkGetter.
 */

namespace Drupal\kalatheme\Utility;

/**
 * Defines configuration used with Kalatheme.
 */
class KalathemeFrameworkGetter {

  /**
   * The Current Active Theme.
   *
   * @var string
   */
  private static $active_theme;

  /**
   * The Current Active Framework Selection.
   *
   * @var string
   */
  private static $active_framework;

  /**
   * The Current Framework Class to use.
   *
   * @var string
   */
  private static $framework_class;

  /**
   * Protected constructor to prevent creating a new instance of this class.
   */
  protected function __construct() {}

  /**
   * Returns the name of the current Active Framework.
   *
   * @return string The current active framework.
   */
  final public static function initialize() {
    // If not statitically set, then grab the active theme.
    if (!isset(static::$active_theme)) {
      // Initialize the active theme.
      static::$active_theme = \Drupal::theme()->getActiveTheme()->getName();
    }

    // If not statitically set, then grab the current framework.
    if (!isset(static::$active_framework)) {
      // Grab the current config and return this as such.
      $config = \Drupal::config('kalatheme.settings');
      static::$active_framework = $config->get('kalatheme_framework');
    }

    // If not statitically set, then grab the current framework class.
    if (!isset(static::$framework_class)) {
      static::$framework_class = ucfirst(static::$active_theme) .
                                 ucfirst(static::$active_framework);
    }

    // return the theme + framework aka the Class to use.
    $class = "\Drupal\kalatheme\Framework\\" . static::$framework_class;
    return new $class() ;
  }
}
