<?php

function kalatheme_theme($existing, $type, $theme, $path){ 
  return array(
    'menu_local_actions' => array(
      'variables' => array('menu_actions' => NULL, 'attributes' => NULL),
    ),
  );
}

function kalatheme_css_alter(&$css) {
  unset($css[drupal_get_path('module','panopoly_admin').'/panopoly-admin.css']);
  unset($css[drupal_get_path('module','panopoly_core').'/css/panopoly-modal.css']);
}

function kalatheme_preprocess_html(&$variables) {
  // Add variables for path to theme.
  $variables['base_path'] = base_path();
  $variables['path_to_kalatheme'] = drupal_get_path('theme', 'kalatheme');
  
  // Add what we need from Bootstrap
  if (module_exists('panopoly_core')) {
    if (($library = libraries_detect('bootstrap')) && !empty($library['installed'])) {
      $bootstrap_path = DRUPAL_ROOT . '/' . $library['library path'];
      $variant = NULL;
      $has_minified_css = file_exists($bootstrap_path . '/css/bootstrap.min.css');
      $has_minified_js = file_exists($bootstrap_path . '/js/bootstrap.min.js');
      if ($has_minified_css && $has_minified_js) {
        $variant = 'minified';
      }
      libraries_load('bootstrap', $variant);
    }
    else {
      // Something went wrong. :(
      // This contains a detailed (localized) error message.
      drupal_set_message(t($library['error message']), 'error');
    }
  }
  else {
    drupal_set_message(t('You need panopoly for this to work.'), 'error');
  }
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
 * Override or insert variables into the page template.
 */
function kalatheme_process_page(&$variables) {
  $dropdown_attributes = array(
    'container' => array(
      'class' => array('dropdown', 'actions', 'pull-right'),
    ),
    'toggle' => array(
      'class' => array('dropdown-toggle', 'enabled'), 
      'data-toggle' => array('dropdown'), 
      'href' => array('#') 
    ),
    'content' => array(
      'class' => array('dropdown-menu'),
    ), 
  );  

  //Add local actions as the last item in the local tasks
  if(!empty($variables['action_links'])){
    $variables['tabs']['#primary'][]['#markup'] = theme('menu_local_actions', array('menu_actions' => $variables['action_links'], 'attributes' => $dropdown_attributes));
    $variables['action_links'] = FALSE; 
  }
  
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
 * Implements hook_preprocess_table()
 */
function kalatheme_preprocess_table(&$variables) {
  if (isset($variables['attributes']['class']) && is_string($variables['attributes']['class'])) {
    $variables['attributes']['class'] = explode(' ', $variables['attributes']['class']);
  }
  $variables['attributes']['class'][] = 'table';
}

/**
 * Implements hook_preprocess_views_view_grid()
 */
function kalatheme_preprocess_views_view_grid(&$variables) {
  if (12 % $variables['options']['columns'] === 0) {
    $variables['span'] = 'span' . 12 / $variables['options']['columns'];
  }
}

/**
 * Implements hook_preprocess_views_view_table()
 */
function kalatheme_preprocess_views_view_table(&$variables) {
  $variables['classes_array'][] = 'table';
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
 * Returns HTML for status and/or error messages, grouped by type.
 */
function kalatheme_status_messages($variables) {
  $display = $variables['display'];
  $output = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
  );

  // Map Drupal message types to their corresponding Bootstrap classes.
  // @see http://twitter.github.com/bootstrap/components.html#alerts
  $status_class = array(
    'status' => 'success',
    'error' => 'error',
    'warning' => 'info',
  );

  foreach (drupal_get_messages($display) as $type => $messages) {
    $class = (isset($status_class[$type])) ? ' alert-' . $status_class[$type] : '';
    $output .= "<div class=\"alert alert-block$class\">\n";
    $output .= "  <a class=\"close\" data-dismiss=\"alert\" href=\"#\">x</a>\n";

    if (!empty($status_heading[$type])) {
      $output .= '<h2 class="element-invisible">' . $status_heading[$type] . "</h2>\n";
    }

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
  }
  return $output;
}


/**
 * Implements hook_libraries_info_alter()
 */
function kalatheme_libraries_info_alter(&$libraries)  {
  $libraries['bootstrap'] = array(
    'name' => 'Twitter Bootstrap',
    'machine name' => 'bootstrap',
    'vendor url' => 'http://twitter.github.com',
    'download url' => 'http://twitter.github.com',
    'path' => '',
    'callbacks' => array(),
    'version arguments' => array(
      'file' => 'css/bootstrap.css',
      'pattern' => '@v+([0-9a-zA-Z\.-]+)@',
      'lines' => 5,
      'cols' => 20,
    ),
    'version callback' => 'libraries_get_version',
    'versions' => array(
      '2' => array(
        'files' => array(
          'js' => array(
            'js/bootstrap.js',
           ),
          'css' => array(
            'css/bootstrap.css',
            'css/bootstrap-responsive.css',
            ),
          ),
          'variants' => array(
            'minified' => array(
              'files' => array(
                'js' => array(
                  'js/bootstrap.min.js',
                ),
                'css' => array(
                  'css/bootstrap.min.css',
                  'css/bootstrap-responsive.min.css',
                ),
              ),
            'variant arguments' => array(
              'variant' => 'minified',
            ),
          ),
        ),
      ),
    ),
  );

  $libraries['bootstrap']['callbacks'] += array(
    'info' => array(),
    'pre-detect' => array(),
    'post-detect' => array(),
    'pre-load' => array(
      'kalatheme_check_responsive',
    ),
    'post-load' => array(),
  );
}

/**
 * Returns HTML for primary and secondary local tasks.
 */
function kalatheme_menu_local_tasks(&$variables) {
  $output = '';

  if ( !empty($variables['primary']) ) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] = '<ul class="nav nav-pills">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }

  if ( !empty($variables['secondary']) ) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['secondary']['#prefix'] = '<ul class="nav nav-pills">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }
  return $output;
}

/**
 * HTML for individual local task links
 */
function kalatheme_menu_local_task($variables){
  $link = $variables['element']['#link'];
  $link_text = $link['title'];
  $classes = array();

  if (!empty($variables['element']['#active'])) {
    // Add text to indicate active tab for non-visual users.
    $active = '<span class="element-invisible">' . t('(active tab)') . '</span>';

    // If the link does not contain HTML already, check_plain() it now.
    // After we set 'html'=TRUE the link will not be sanitized by l().
    if (empty($link['localized_options']['html'])) {
      $link['title'] = check_plain($link['title']);
    }
    $link['localized_options']['html'] = TRUE;
    $link_text = t('!local-task-title!active', array('!local-task-title' => $link['title'], '!active' => $active));

    $classes[] = 'active';
  }

  return '<li class="' . implode(' ', $classes) . '">' . l($link_text, $link['href'], $link['localized_options']) . "</li>\n";
}

/**
 * HTML for all local actions (rendered as dropdown)
 */
function kalatheme_menu_local_actions($variables){
  $container_attributes = isset($variables['attributes']['container']) ? drupal_attributes($variables['attributes']['container']) : FALSE;
  $toggle_attributes = isset($variables['attributes']['toggle']) ? drupal_attributes($variables['attributes']['toggle']) : FALSE;
  $content_attributes = isset($variables['attributes']['content']) ? drupal_attributes($variables['attributes']['content']) : FALSE;

  //Render the dropdown
  $output = $container_attributes ?  '<li' . $container_attributes . '>' : '<li>';
  $output .= $toggle_attributes ?  '<a' . $toggle_attributes . '><i class="icon-wrench"></i> Actions <b class="caret"></b></a>' : '<a>Actions <b class="caret"></b></a>'; 
  $output .= $content_attributes ? '<ul' . $content_attributes . '>' : '<ul>';
  $output .= drupal_render($variables['menu_actions']);
  $output .= '</ul>';
  $output .= '</li>';
  
  return $output;
}

/**
 * HTML for individual local actions
 */
function kalatheme_menu_local_action($variables){
  $link = $variables['element']['#link'];

  $output = '<li>';
  if (isset($link['href'])) {
    $output .= l($link['title'], $link['href'], isset($link['localized_options']) ? $link['localized_options'] : array());
  }
  elseif (!empty($link['localized_options']['html'])) {
    $output .= $link['title'];
  }
  else {
    $output .= check_plain($link['title']);
  }
  $output .= "</li>\n";

  return $output;
}

/**
 * Checks if Bootstrap's responsive CSS is installed
 *
 * @param array $variant
 *   Library, or one of its variants, to check
 * @param $version
 *   Library's version number, if applicable
 * @param $variant_name
 *   Name of current variant, if applicable
 */
function kalatheme_check_responsive(&$variant, $version, $variant_name) {
  foreach ($variant['files']['css'] as $index => $css) {
    if (!preg_match('/^css\/bootstrap\-responsive\.(?:min\.)?css$/', $css)) {
      continue;
    }
    $css_path = DRUPAL_ROOT . '/' . $variant['library path'] . '/' . $css;
    if (!file_exists($css_path)) {
      unset($variant['files']['css'][$index]);
    }
  }
}

