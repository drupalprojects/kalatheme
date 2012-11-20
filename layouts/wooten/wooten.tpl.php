<?php
/**
 * @file
 * Template for Kalatheme Wooten.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display wooten clearfix <?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  
  <section class='section alt' id='promo'>
    <div class='container'>
      <div class="wooten-container wooten-header clearfix panel-panel">
        <div class="wooten-container-inner wooten-header-inner panel-panel-inner">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>
  
  <section class='section'>
    <div class='container'>
      <div class="wooten-container wooten-header clearfix panel-panel row-fluid">
        <div class="wooten-container-inner wooten-header-inner panel-panel-inner">
          <?php print $content['main']; ?>
        </div>
      </div>
    </div>
  </section>
  
  <section class='section alt'>
    <div class='container'>
      <div class="wooten-container wooten-header clearfix panel-panel row-fluid">
        <div class="wooten-container-inner wooten-header-inner panel-panel-inner">
          <?php print $content['second']; ?>
        </div>
      </div>
    </div>
  </section>
  
  <footer class='section' id='footer' role='contentinfo'>
    <div class='container'>
      <div class="wooten-container wooten-column-content wooten-column-content-row1 clearfix row-fluid">
        <div class="wooten-column-content-region wooten-column wooten-column1 panel-panel span4">
          <div class="wooten-column-content-region-inner wooten-column-inner wooten-column1-inner panel-panel-inner">
            <?php print $content['footer1']; ?>
          </div>
        </div>
        <div class="wooten-column-content-region wooten-column wooten-column2 panel-panel span4">
          <div class="wooten-column-content-region-inner wooten-column-inner wooten-column2-inner panel-panel-inner">
            <?php print $content['footer2']; ?>
          </div>
        </div>
        <div class="wooten-column-content-region wooten-column wooten-column3 panel-panel span4">
          <div class="wooten-column-content-region-inner wooten-column-inner wooten-column3-inner panel-panel-inner">
            <?php print $content['footer3']; ?>
          </div>
        </div>
      </div>
    </div>
  </footer>
</div><!-- /.wooten -->
