<?php

/**
 * @file
 * Contains \Drupal\kalatheme\Theme\KalathemeBase.
 */

namespace Drupal\kalatheme\Theme;


/**
 * Defines base class to be used with all Framework Classes.
 */
abstract class KalathemeBase {

  /**
   * The current active framwork.
   */
  protected $framework;

  /**
   * Gets the current active framework base name.
   */
  protected function getFramework() {
    if (!isset($this->framework)) {
      $this->setFramework();
    }
    return $this->framework;
  }

  /**
   * Sets the current active framework base name.
   */
  protected function setFramework() {
    $this->framework = theme_get_setting('kalatheme_framework');
  }

  /**
   * Returns the dynamically build library based on framework.
   *
   * @return array
   *   An array of library definitions to register, keyed by library ID.
   *
   * @see hook_library_info_build
   */
  public function libraryBuild() {
    $libraries = [];

    // If CDN do things n stuff.
    if (theme_get_setting('kalatheme_' . $this->getFramework() . '_enable_cdn')) {
      // Load the theme settings for the CDN.
      $js = theme_get_setting('kalatheme_' . $this->getFramework() . '_cdn_js');
      $css = theme_get_setting('kalatheme_' . $this->getFramework() . '_cdn_css');

      // Add them to the library.
      $libraries[$this->getFramework()] = [
        'js' => [
          $js => [
            'type' => 'external',
            'minified' => 'true',
          ],
        ],
        'css' => [
          'component' => [
            $css => [
              'type' => 'external',
              'minified' => 'true',
            ],
          ],
        ],
      ];
    }
    // Send this all back to win.
    return $libraries;
  }

  /**
   * Return altered attachments to a page before it is rendered.
   *
   * @return array
   *   An associative array of the same format as returned by
   *   hook_page_attachments_alter().
   *
   * @see hook_page_attachments_alter
   */
  public function preprocessPageAttachments(array &$page) {
    // Grab the theme setting for the viewport.
    $viewport_content = theme_get_setting('kalatheme_viewport');
    $viewport = [
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => [
        'name' => 'viewport',
        'content' => $viewport_content,
      ],
    ];
    $page['#attached']['html_head'][] = [$viewport, 'viewport'];

    // Set the library to the current framework settings.
    $page['#attached']['library'][] = 'kalatheme/' . $this->getFramework();
  }
}
