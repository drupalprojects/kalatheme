<?php

/**
 * @file
 * Contains \Drupal\kalatheme\Theme\KalathemeBase.
 */

namespace Drupal\kalatheme\Theme;

use Drupal\block\Entity\Block;

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
    $libraries[$this->getFramework()] = [];

    // If CDN do things n stuff.
    if (theme_get_setting('kalatheme_' . $this->getFramework() . '_enable_cdn')) {
      // Load the theme settings from the CDN.
      $js = theme_get_setting('kalatheme_' . $this->getFramework() . '_cdn_js');
      $css = theme_get_setting('kalatheme_' . $this->getFramework() . '_cdn_css');

      // Add them to the library.
      $libraries[$this->getFramework()] += [
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
    // Check for fontawesome.
    if (theme_get_setting('kalatheme_fontawesome_enable')) {
      // Load the fontawesome settings from the CDN.
      $css = theme_get_setting('kalatheme_fontawesome_cdn');

      // Add fontawesome to the library.
      $libraries[$this->getFramework()]['css']['component'] += [
        $css => [
          'type' => 'external',
          'minified' => 'true',
        ],
      ];
    }
    // Check for foundation icons.
    if (theme_get_setting('kalatheme_foundationicon_enable')) {
      // Load the foundation settings from the CDN.
      $css = theme_get_setting('kalatheme_foundationicon_cdn');

      // Add foundation to the library.
      $libraries[$this->getFramework()]['css']['component'] += [
        $css => [
          'type' => 'external',
          'minified' => 'true',
        ],
      ];
    }
    // Send this all back to win.
    return $libraries;
  }

  /**
   * Return altered variables for the page template.
   *
   * @return array
   *   An associative array of the same format as returned by
   *   template_preprocess_page().
   *
   * @see template_preprocess_page
   */
  public function preprocessPage(array &$variables) {
    // Get active theme
    $theme = \Drupal::theme()->getActiveTheme()->getName();

    // Load all the blocks to parse through.
    $blocks = Block::loadMultiple();
    // We are adding all the blocks so we can.
    // achieve for the One content region setup.
    foreach ($blocks as $key => $block) {
      $check = strstr($key, '_', TRUE);
      // Check is the block name matches the theme.
      if ($check == $theme) {
        $name = ltrim(strstr($key, '_'), '_');
        // Load the block as a var to use in the page.html.twig.
        $variables[$name] = \Drupal::entityManager()
        ->getViewBuilder('block')
        ->view($block);
      }
    }
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
