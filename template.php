<?php

/**
 * Constants
 */
// Define a default fade in speed.
define('PURR_FADE_IN', 1200);
// Define a default fade out speed.
define('PURR_FADE_OUT', 2000);
// Define a default timeout speed.
define('PURR_TIMER', 5000);
// Show this notice on every page except the listed pages.
define('PURR_VISIBILITY_NOTLISTED', 0);
// Show this notice on only the listed pages.
define('PURR_VISIBILITY_LISTED', 1);
// Show this notice if the associated PHP code returns TRUE.
define('PURR_VISIBILITY_PHP', 2);
// Which DOM element to attach to.
define('PURR_ATTACH_TO', 'body');
// Whether notices should be sticky or not.
define('PURR_STICKY', FALSE);

function kalatheme_preprocess_html(&$variables) {
  // Add variables for path to theme.
  $variables['base_path'] = base_path();
  $variables['path_to_kalatheme'] = drupal_get_path('theme', 'kalatheme');
  $variables['classes_array'][] = 'theme-pattern-lightmesh';
}

/**
 * Override or insert variables into the page template for HTML output.
 */
function kalatheme_process_html(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($variables);
  }
}

/**
 * Implements template_preprocess_page().
 */
function kalatheme_preprocess_page(&$variables) {
  //Add what we need from Bootstrap
  _kalatheme_add_bootstrap_css();
}

/**
 * Override or insert variables into the page template.
 */
function kalatheme_process_page(&$variables) {
  // Get the entire main menu tree
  $main_menu_tree = array();
  $main_menu_tree = menu_tree_all_data('main-menu', NULL, 2);
  // Add the rendered output to the $main_menu_expanded variable
  $variables['main_menu_expanded'] = menu_tree_output($main_menu_tree);
  
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty, so we rebuild it.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($variables['title_suffix']['add_or_remove_shortcut']) && $variables['title']) {
    // Add a wrapper div using the title_prefix and title_suffix render elements.
    $variables['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $variables['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
    );
    // Make sure the shortcut link is the first item in title_suffix.
    $variables['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }
  
  // if panels arent being used at all
  if (!isset($variables['page']['content']['system_main']['main']['#markup']) || (strpos($variables['page']['content']['system_main']['main']['#markup'], 'panel-panel') === FALSE)) {
    $variables['kala_contain'] = 'container';
    $variables['buffer'] = 'buffer';
  }
}

/**
 * Implements hook_preprocess_maintenance_page().
 */
function kalatheme_preprocess_maintenance_page(&$variables) {
  // By default, site_name is set to Drupal if no db connection is available
  // or during site installation. Setting site_name to an empty string makes
  // the site and update pages look cleaner.
  // @see template_preprocess_maintenance_page
  if (!$variables['db_is_active']) {
    $variables['site_name'] = '';
  }
  drupal_add_css(drupal_get_path('theme', 'kalatheme') . '/css/maintenance-page.css');
}

/**
 * Override or insert variables into the maintenance page template.
 */
function kalatheme_process_maintenance_page(&$variables) {
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty, so we rebuild it.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
}

/**
 * Override or insert variables into the node template.
 */
function kalatheme_preprocess_node(&$variables) {
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
}

/**
 * Override or insert variables into the block template.
 */
function kalatheme_preprocess_block(&$variables) {
  // In the header region visually hide block titles.
  if ($variables['block']->region == 'header') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
}

/**
 * Implements hook_preprocess_views_view()
 */
function kalatheme_preprocess_views_view_grid(&$variables) {
  if (12 % $variables['options']['columns'] === 0) {
    $variables['span'] = 'span' . 12 / $variables['options']['columns'];
  }
}

/**
 * Implements theme_form()
 */
function kalatheme_form($variables) {
  $element = $variables['element'];
  if (isset($element['#action'])) {
    $element['#attributes']['action'] = drupal_strip_dangerous_protocols($element['#action']);
  }
  element_set_attributes($element, array('method', 'id'));
  if (empty($element['#attributes']['accept-charset'])) {
    $element['#attributes']['accept-charset'] = "UTF-8";
  }
  // Anonymous DIV to satisfy XHTML compliance.
  return '<form' . drupal_attributes($element['#attributes']) . '><fieldset>' . $element['#children'] . '</fieldset></form>';
}

/**
 * Implements theme_button()
 */
function kalatheme_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  $element['#attributes']['class'][] = 'btn';
  $element['#attributes']['class'][] = 'btn-primary';
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }
  
  return '<input' . drupal_attributes($element['#attributes']) . ' />';;
}

/**
 * Implements theme_textarea()
 */
function kalatheme_textarea($variables) {
  $element = $variables['element'];
  $element['#attributes']['name'] = $element['#name'];
  $element['#attributes']['id'] = $element['#id'];
  $element['#attributes']['cols'] = $element['#cols'];
  $element['#attributes']['rows'] = $element['#rows'];
  _form_set_class($element, array('form-textarea'));

  $wrapper_attributes = array(
      'class' => array('form-textarea-wrapper'),
  );

  // Add resizable behavior.
  if (!empty($element['#resizable'])) {
    $wrapper_attributes['class'][] = 'resizable';
  }

  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';
  return $output;
}

/**
 * Implements theme_menu_tree().
 */
function kalatheme_menu_tree($variables) {
  return '<ul class="menu clearfix">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_field__field_type().
 */
function kalatheme_field__taxonomy_term_reference($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<h3 class="field-label">' . $variables['label'] . ': </h3>';
  }

  // Render the items.
  $output .= ($variables['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($variables['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . (!in_array('clearfix', $variables['classes_array']) ? ' clearfix' : '') . '"' . $variables['attributes'] .'>' . $output . '</div>';

  return $output;
}


/**
 * Implements theme_links__system_secondary_menu().
 */
function kalatheme_links__system_main_menu($variables) {
  $links = $variables['links'];
  $attributes = $variables['attributes'];
  $heading = $variables['heading'];
  unset($links['#sorted']);
  unset($links['#theme_wrappers']);
  global $language_url;
  $output = '';

  if (count($links) > 0) {
    $output = '';

    // Treat the heading first if it is present to prepend it to the
    // list of links.
    if (!empty($heading)) {
      if (is_string($heading)) {
        // Prepare the array that will be used when the passed heading
        // is a string.
        $heading = array(
          'text' => $heading,
          // Set the default level of the heading. 
          'level' => 'h2',
        );
      }
      $output .= '<' . $heading['level'];
      if (!empty($heading['class'])) {
        $output .= drupal_attributes(array('class' => $heading['class']));
      }
      $output .= '>' . check_plain($heading['text']) . '</' . $heading['level'] . '>';
    }

    $output .= '<ul' . drupal_attributes($attributes) . '>';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $class = array($key);
      
      // Add first, last and active classes to the list of links to help out themers.
      if ($i == 1) {
        $class[] = 'first';
      }
      if ($i == $num_links) {
        $class[] = 'last';
      }
      if (isset($link['#href']) && ($link['#href'] == $_GET['q'] || ($link['#href'] == '<front>' && drupal_is_front_page()))
           && (empty($link['#language']) || $link['#language']->language == $language_url->language)) {
        $class[] = 'active';
      }
      if (!empty($link['#below'])) {
        $class[] = 'dropdown';
        $link['#attributes']['data-toggle'] = 'dropdown';
        $link['#attributes']['class'][] = 'dropdown-toggle';
        $link['#href'] = NULL;
      }
      $options['attributes'] = $link['#attributes'];
      
      $output .= '<li' . drupal_attributes(array('class' => $class)) . '>';

      if (isset($link['#href'])) {
        // Pass in $link as $options, they share the same keys.
        $output .= l($link['#title'], $link['#href'], array('attributes' => $link['#attributes']));
      }
      // need to put in empty anchor for dropdown, for some reason drupal can't do this with l()?
      elseif ($link['#attributes']['data-toggle'] && !isset($link['#href'])) {
        $output .= str_replace('href="/"', 'href="#"', l($link['#title'], $link['#href'], array('attributes' => $link['#attributes'])));
      }
      elseif (!empty($link['#title'])) {
        // Some links are actually not links, but we wrap these in <span> for adding title and class attributes.
        if (empty($link['#html'])) {
          $link['#title'] = check_plain($link['#title']);
        }
        $span_attributes = '';
        if (isset($link['#attributes'])) {
          $span_attributes = drupal_attributes($link['#attributes']);
        }
        $output .= '<span' . $span_attributes . '>' . $link['#title'] . '</span>';
      }
      
      if (!empty($link['#below'])) {
        $output .= theme('links__system_main_menu', array(
                    'links' => $link['#below'],
                      'attributes' => array(
                        'class' => array('dropdown-menu'),
                      ),
                  ));
      }

      $i++;
      $output .= "</li>\n";
    }

    $output .= '</ul>';
  }

  return $output;
}

/**
 * 
 * @param unknown $variables
 * @return string
 */
function kalatheme_status_messages($variables) {
  $display = $variables['display'];
  $output = '';
  $purr = NULL;
  foreach (drupal_get_messages($display) as $type => $messages) {
    $purr[] = _kalatheme_messages_purr($type, array_unique($messages));
  }
  $theme_path = drupal_get_path('theme', 'kalatheme');
  drupal_add_css($theme_path . '/css/messages.css');

  if ($purr) {
    // Add the purr js
    drupal_add_js($theme_path . '/js/jquery.timer.js');
    drupal_add_js($theme_path . '/js/jquery.purr.js');
    // Add the settings
    $settings = array(
      'fadeInSpeed' => variable_get('purr_messages_fade_in', PURR_FADE_IN),
      'fadeOutSpeed' => variable_get('purr_messages_fade_out', PURR_FADE_OUT),
      'removeTimer' => variable_get('purr_messages_timer', PURR_TIMER),
      'pauseOnHover' => variable_get('purr_messages_hover', TRUE) ? TRUE : FALSE,
      'usingTransparentPNG' => variable_get('purr_messages_transparent', TRUE) ? TRUE : FALSE,
      'attachTo' => variable_get('purr_messages_attachto', PURR_ATTACH_TO),
      'sticky' => variable_get('purr_messages_sticky', PURR_STICKY),
    );
    drupal_add_js(array('purr_messages' => $settings), 'setting');
    $output .= "<script type=\"text/javascript\">";
    $output .= "(function($) {\nDrupal.behaviors.purr_messages = {\n
      attach: function(context) {\n if ($('#purr-container').length == 0) { \nvar notice = ";
    foreach ($purr as $purr_message) {
      $script[] = $purr_message['script'];
    }
    $script = array_unique($script);
    $output .= implode(' + ', $script);
    $output .= "$(notice).purr();";
    // Finish off the script.
    $output .= "\n}\n}\n}\n})(jQuery);</script>\n";
    $output .= "<noscript>\n";
    foreach ($purr as $purr_message) {
      $output .= $purr_message['noscript'];
    }
    $output .= "</noscript>\n";
  }
  return $output;
}


/**
 * Builds and returns the formatted purr message code
 *
 * @param $type
 *   String containing a message type. Used to set the class on the message div.
 *
 * @param $messages
 *   An array, each containing a message.
 *
 * @return array A string containing the formatted messages.
 */
function _kalatheme_messages_purr($type, $messages) {
  $script = '';
  $pattern = array("\r\n", "\r", "\n", "\t");
  $script .= "'<div class=\"notice $type\">'\n + '<div class=\"notice-body\">'";
  if (count($messages) > 1) {
    $script .= "+ '<ul>'\n";
    foreach ($messages as $message) {
      $script .= "+  '<li>" . str_replace($pattern, ' ', addslashes($message)) . "</li>'\n";
    }
    $script .= "+ '</ul>'\n";
  }
  else {
    $script .= "\n+ '" . str_replace($pattern, ' ', addslashes($messages[0])) . "'\n";
  }
  $script .= "+ '</div>'\n + '<div class=\"notice-bottom\">'\n +
    '</div>' + '</div>'\n";
  $output['script'] = $script;
  $output['noscript'] = theme('original_status_messages', array('type' => $type, 'messages' => $messages));
  return $output;
}


/**
 * Adds the Bootstrap CSS that Kalatheme needs for every page
 */
function _kalatheme_add_bootstrap_css() {
  $bootstrap_files = array(
    'reset.css', 'scaffolding.css', 'grid.css', 'layouts.css', 'type.css',
    'forms.css', 'tables.css', 'sprites.css', 'wells.css', 'buttons.css',
    'alerts.css', 'navs.css', 'navbar.css', 'thumbnails.css', 'media.css',
    'hero-unit.css', 'utilities.css', 'responsive.css',
  );
  $bootstrap_css_path = libraries_get_path('bootstrap') . '/css/';
  foreach ($bootstrap_files as $file) {
    drupal_add_css($bootstrap_css_path . $file, array(
      'every_page' => TRUE,
    ));
  }
}


/**
 * Return a themed set of status and/or error messages. The messages are grouped
 * by type.
 *
 * This is the original output which we use if purr messages is turned off.
 *
 * @param $vars
 *
 * @internal param $type String containing a message type. Used to set the class on the message div.
 *
 * @internal param $messages An array, each containing a message.
 *
 * @return string A string containing the formatted messages.
 */
function theme_original_status_messages($vars) {
  $type = $vars['type'];
  $messages = $vars['messages'];
  $output = '';
  $output .= "<div class=\"messages $type\">\n";
  if (count($messages) > 1) {
    $output .= " <ul>\n";
    foreach ($messages as $message) {
      $output .= '  <li>' . $message . "</li>\n";
    }
    $output .= " </ul>\n";
  }
  else {
    $output .= $messages[0];
  }
  $output .= "</div>\n";
  return $output;
}

/**
 * Implements hook_libraries_info()
 */
function kalatheme_libraries_info() {
  return array(
    'bootstrap' => array(
      'name' => 'Twitter Bootstrap',
      'vendor url' => 'http://twitter.github.com',
      'download url' => 'https://github.com/twitter/bootstrap/tarball/v2.0.3',
    ),
  );
}
