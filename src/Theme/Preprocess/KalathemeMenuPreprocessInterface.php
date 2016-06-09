<?php
/**
 * @file
 * Contains \Drupal\kalatheme\Theme\Preprocess\KalathemeMenuPreprocessInterface.
 */

namespace Drupal\kalatheme\Theme\Preprocess;

/**
 * Contains the interface for some of the menu type preprocessors.
 *
 * Not all are in here and are defined in KalthemeBase for specific needs.
 *
 * @see \Drupal\kalatheme\Theme\KalathemeBase
 *
 * Gives the structure to manipulate in the frameworks.
 */
interface KalathemeMenuPreprocessInterface {

  /**
   * Return altered variables for TWIG Template.
   *
   * @return array
   *   An associative array of the same format as returned by
   *   template_preprocess_menu__main().
   *
   * @see template_preprocess_menu__main()
   * @see menu--main.html.twig
   */
  public function preprocessMenuMain(array &$variables);

  /**
   * Return altered variables for TWIG Template.
   *
   * @return array
   *   An associative array of the same format as returned by
   *   template_preprocess_menu__secondary().
   *
   * @see template_preprocess_menu__secondary()
   * @see menu--secondary.html.twig
   */
  public function preprocessMenuSecondary(array &$variables);

  /**
   * Return altered variables for TWIG Template.
   *
   * @return array
   *   An associative array of the same format as returned by
   *   template_preprocess_menu_tree().
   *
   * @see template_preprocess_menu_tree()
   * @see menu-tree.html.twig
   */
  public function preprocessMenuTree(array &$variables);

  /**
   * Return altered variables for TWIG Template.
   *
   * @return array
   *   An associative array of the same format as returned by
   *   template_preprocess_menu_link().
   *
   * @see template_preprocess_menu_link()
   * @see menu-link.html.twig
   */
  public function preprocessMenuLink(array &$variables);

  /**
   * Return altered variables for TWIG Template.
   *
   * @return array
   *   An associative array of the same format as returned by
   *   template_preprocess_menu_local_tasks().
   *
   * @see template_preprocess_menu_local_tasks()
   * @see menu-local-tasks.html.twig
   */
  public function preprocessMenuLocalTasks(array &$variables);

  /**
   * Return altered variables for TWIG Template.
   *
   * @return array
   *   An associative array of the same format as returned by
   *   template_preprocess_menu_local_task().
   *
   * @see template_preprocess_menu_local_task()
   * @see menu-local-task.html.twig
   */
  public function preprocessMenuLocalTask(array &$variables);

  /**
   * Return altered variables for TWIG Template.
   *
   * @return array
   *   An associative array of the same format as returned by
   *   template_preprocess_menu_local_actions().
   *
   * @see template_preprocess_menu_local_actions()
   * @see menu-local-actions.html.twig
   */
  public function preprocessMenuLocalActions(array &$variables);

  /**
   * Return altered variables for TWIG Template.
   *
   * @return array
   *   An associative array of the same format as returned by
   *   template_preprocess_menu_local_action().
   *
   * @see template_preprocess_menu_local_action()
   * @see menu-local-action.html.twig
   */
  public function preprocessMenuLocalAction(array &$variables);
}
