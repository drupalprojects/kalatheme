<?php
/**
 * @file
 * Template for Kalatheme Hastings.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display hastings clearfix <?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  
  <section class='section alt' id='promo'>
    <div class='container'>
      <div class="hastings-container hastings-header clearfix panel-panel">
        <div class="hastings-container-inner hastings-header-inner panel-panel-inner">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>
  
  <section class='section'>
    <div class='container'>
      <div class="hastings-container hastings-header clearfix panel-panel row-fluid">
        <div class="hastings-container-inner hastings-header-inner panel-panel-inner">
          <?php print $content['main']; ?>
        </div>
      </div>
    </div>
  </section>
  
  <footer class='section' id='footer' role='contentinfo'>
    <div class='container'>
      <div class="hastings-container hastings-column-content hastings-column-content-row1 clearfix row-fluid">
        <div class="hastings-column-content-region hastings-column hastings-column1 panel-panel span4">
          <div class="hastings-column-content-region-inner hastings-column-inner hastings-column1-inner panel-panel-inner">
            <?php print $content['footer1']; ?>
          </div>
        </div>
        <div class="hastings-column-content-region hastings-column hastings-column2 panel-panel span4">
          <div class="hastings-column-content-region-inner hastings-column-inner hastings-column2-inner panel-panel-inner">
            <?php print $content['footer2']; ?>
          </div>
        </div>
        <div class="hastings-column-content-region hastings-column hastings-column3 panel-panel span4">
          <div class="hastings-column-content-region-inner hastings-column-inner hastings-column3-inner panel-panel-inner">
            <?php print $content['footer3']; ?>
          </div>
        </div>
      </div>
    </div>
  </footer>
</div><!-- /.hastings -->
