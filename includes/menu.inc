<?php
/**
 * @file
 * Theme and hook functions for menu things.
 */

/**
 * Implements theme_menu_tree().
 */
function kalatheme_menu_tree($variables) {
  // Set the aria role to menu
  return '<ul class="menu clearfix" role="menu">' . $variables['tree'] . '</ul>';
}


/**
 * Implements theme_menu_link().
 * Aria rolese added to the link
 */
function kalatheme_menu_link($variables) {
  $element = $variables['element'];
  $sub_menu = '';
  $element['#attributes']['role'] = 'presentation';
  // Enforce aria-role="menuitem"
  $role = array('role'=> 'menuitem' );
  $element['#localized_options']['attributes'] = isset($element['#localized_options']['attributes']) ? $element['#localized_options']['attributes']
   +=  $role : $role;
  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}
/**
 * Implements theme_links__system_main_menu.
 */
function kalatheme_links__system_main_menu($variables) {
  $links = $variables['links'];
  $attributes = $variables['attributes'];

  // Navbar best described by menubar, only add attribute if role="menu" isn't set
  if(!isset($attributes['role'])){
    $attributes['role'] = 'menubar';
  }
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
          'level' => 'h3',
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
      $class = array('mid-' . $key);

      //Pass in the original link options to include attributes and fragments
      $link['#options'] = $link['#original_link']['options'];
      //Makes sure the attributes are set
      if( empty($link['#options']['attributes']) ){
        $link['#options']['attributes'] = array();
      }

      // Merge in the attributes
      $link['#options']['attributes'] += $link['#attributes'];

      // Merge in more attributes if they are set
      if ( isset($link['#localized_options']['attributes'])){
        $link['#options']['attributes'] += $link['#localized_options']['attributes'];
      }
      // lastly add in the role=menuitem for links
      $link['#options']['attributes']['role'] = 'menuitem';
      // Add first/last/active classes to help out themers.
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
      // Check for, and honor active states set by context menu reactions
      if (module_exists('context')) {
        $contexts = context_active_contexts();
        foreach ($contexts as $context) {
          if ((isset($context->reactions['menu']))) {
            if ($link['#href'] == $context->reactions['menu']) {
              $class[] = 'active';
            }
          }
        }
      }

      // Make list item a dropdown if we have child items.
      if (!empty($link['#below'])) {
        $class[] = 'dropdown';
        $class[] = 'clearfix';
        // print a toggle.
        $link['#options']['attributes']['role'] = array("button");
        $link['#options']['attributes']['class'] += array("dropdown-toggle");
        $link['#options']['attributes']['id'] = "main-menu-dropdown-" . $i;
        $link['#options']['attributes']['data-toggle'] = array("dropdown");
        $link['#options']['attributes']['aria-haspopup'] = array("true");
        $link['#options']['html'] = true;
        $link['#title'] .= '<span class="caret" aria-hidden="true"></span>';
      }
      $output .= '<li' . drupal_attributes(array('class' => $class, 'role' =>'presentation')) . '>';

      if (isset($link['#href'])) {
        // Pass in $link with the path and the options set to the l() function
        $output .= l($link['#title'], $link['#href'], $link['#options']);
      }
      elseif (!empty($link['#title'])) {
        // Wrap non-<a> links in <span> for adding title and class attributes.
        if (empty($link['#html'])) {
          $link['#title'] = check_plain($link['#title']);
        }
        $span_attributes = '';
        if (isset($link['#attributes'])) {
          $span_attributes = drupal_attributes($link['#attributes']);
        }
        $output .= '<span' . $span_attributes . '>' . $link['#title'] . '</span>';
      }

      $clearfix = '';
      // If link has child items, print a dropdown menu.
      if (!empty($link['#below'])) {
        $dropdown_id = 'main-menu-dropdown-' . $i;
        //Add in text to be available for AT
        $output .= '<span class="element-invisible">Toggle submenu for ' . strip_tags($link['#title']) . '</span></a>';
        $output .= theme('links__system_main_menu', array(
          'links' => $link['#below'],
          'attributes' => array(
            'class' => array('dropdown-menu'),
            'role' => 'menu',
            'aria-labelledby' => $dropdown_id,
            'aria-expanded' => 'false'
          ),
        ));
        $clearfix = "<div class='clearfix visible-xs'></div>";
      }

      $i++;
      $output .= "</li>" . $clearfix . "\n";
    }

    $output .= '</ul>';
  }

  return $output;
}

/**
 * Returns HTML for primary and secondary local tasks.
 *
 * Implements theme_menu_local_tasks().
 */
function kalatheme_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h3 class="element-invisible">' . t('Primary tabs') . '</h3>';
    $variables['primary']['#prefix'] = '<ul class="nav nav-pills" role="menu">';
    $variables['primary']['#suffix'] = '</ul>';
    if (!empty($variables['secondary'])) {
      $variables = _kalatheme_associate_parent_tasks($variables);
    }
    $output .= drupal_render($variables['primary']);
  }

  return $output;
}

/**
 * HTML for individual local task links.
 *
 * Implements theme_menu_local_task().
 */
function kalatheme_menu_local_task($variables) {
  $link = $variables['element']['#link'];
  $children = isset($variables['element']['#children']) ? $variables['element']['#children'] : FALSE;
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

  // If the primary link has children, render them as a dropdown.
  if ($children) {
    $classes[] = 'dropdown';
    $link['localized_options']['attributes']['class'][] = 'dropdown-toggle';
    $link['localized_options']['attributes']['data-toggle'][] = 'dropdown';
    $link['localized_options']['attributes']['role'] = 'button';
    $link['href'] = '#';
    $link_text .= ' <span class="caret" aria-hidden="true"></span>';
    $output = '<li class="' . implode(' ', $classes) . '" role="presentation">';
    $output .= l($link_text, $link['href'], $link['localized_options']);
    $output .= '<ul class="dropdown-menu" role="menu">';
    $output .= drupal_render($children);
    $output .= '</ul>';
    $output .= '</li>';
    return $output;
  }
  else {
    $options = $link['localized_options'];
    $role= array('role'=> 'menuitem');
    $options['attributes'] = isset( $options['attributes'] ) ? $options['attributes'] += $role : $role;
    return '<li class="' . implode(' ', $classes) . '" role="presentation">' . l($link_text, $link['href'], $link['localized_options']) . "</li>\n";
  }
}

/**
 * HTML for all local actions (rendered as dropdown).
 */
function kalatheme_menu_local_actions($variables) {
  $container_attributes = isset($variables['attributes']['container']) ? drupal_attributes($variables['attributes']['container']) : FALSE;
  $toggle_attributes = isset($variables['attributes']['toggle']) ? drupal_attributes($variables['attributes']['toggle']) : FALSE;
  $content_attributes = isset($variables['attributes']['content']) ? drupal_attributes($variables['attributes']['content']) : FALSE;

  // Render the dropdown.
  $output = $container_attributes ?  '<li' . $container_attributes . '>' : '<li>';
  $output .= $toggle_attributes ?  '<a role="button"' . $toggle_attributes . '><span class="glyphicon glyphicon-wrench fa fa-wrench" aria-hidden="true"></span> ' . t('Actions') . '<span class="caret" aria-hidden="true"></span></a>' : '<a role="button">' . t('Actions') . '<span class="caret" aria-hidden="true"></span></a>';
  $output .= $content_attributes ? '<ul' . $content_attributes . ' role="menu">' : '<ul role="menu">';
  $output .= drupal_render($variables['menu_actions']);
  $output .= '</ul>';
  $output .= '</li>';

  return $output;
}

/**
 * HTML for individual local actions.
 */
function kalatheme_menu_local_action($variables) {
  $link = $variables['element']['#link'];

  $output = "<li role=\"presenation\">";
  if (isset($link['href'])) {
    $options = isset($link['localized_options']) ? $link['localized_options'] : array();
    // Adding ARIA Attributes for the link as a menuitem
    // @link http://www.w3.org/TR/wai-aria/roles#menuitem
    $a11y_attr =  array(
      'role'=>'menuitem',
      'tab-index' => '-1'
    );
    $options['attributes'] = isset($options['attributes']) ? $options['attributes'] += $a11y_attr : $a11y_attr;

    $output .= l($link['title'], $link['href'], $options);
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
 * Helper function to filter element children.
 *
 * See http://api.drupal.org/comment/34713#comment-34713
 */
function _kalatheme_element_children($element) {
  return array_intersect_key(
    $element,
    array_flip(element_children($element))
  );
}

/**
 * Associate all the secondary menu tasks as #children of primary tasks.
 */
function _kalatheme_associate_parent_tasks($variables) {
  // Assign all secondary links as children of their parent primary link.
  foreach (_kalatheme_element_children($variables['secondary']) as $secondary_index => $secondary_link) {
    $link = $secondary_link['#link'];
    foreach (_kalatheme_element_children($variables['primary']) as $primary_index => $primary_link) {
      if (!isset($primary_link['#markup']) && $primary_link['#link']['path'] == $link['tab_parent']) {
        $variables['primary'][$primary_index]['#children'][] = $secondary_link;
        unset($variables['secondary'][$secondary_index]);
      }
    }
  }

  // If a secondary link hasn't been assigned, make it a primary link.
  // @todo: this should never happen; consider removing?
  foreach ($variables['secondary'] as $secondary_link) {
    $variables['primary'][] = $secondary_link;
  }

  return $variables;
}
