<?php
/**
 * @file
 * Template for Panopoly Pond.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display pond clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>

  <section class='section alt' id='promo'>
    <div class='container'>
      <div class="pond-container pond-header clearfix panel-panel ">
        <div class="pond-container-inner pond-header-inner panel-panel-inner">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>
  
  <section class='section'>
    <div class='container'>      
      <div class="pond-container pond-column-content pond-column-content-row1 clearfix row-fluid">
        <div class="pond-column-content-region pond-column pond-column1 panel-panel span4">
          <div class="pond-column-content-region-inner pond-column-inner pond-column1-inner panel-panel-inner">
            <?php print $content['column1']; ?>
          </div>
        </div>
        <div class="pond-column-content-region pond-column pond-column2 panel-panel span4">
          <div class="pond-column-content-region-inner pond-column-inner pond-column2-inner panel-panel-inner">
            <?php print $content['column2']; ?>
          </div>
        </div>
        <div class="pond-column-content-region pond-column pond-column3 panel-panel span4">
          <div class="pond-column-content-region-inner pond-column-inner pond-column3-inner panel-panel-inner">
            <?php print $content['column3']; ?>
          </div>
        </div>
      </div>
    </div>
  </section>
  
  <section class='section'>
    <div class='container'>      
      <div class="pond-container pond-secondary-column-content pond-column-content-row2 clearfix row-fluid">
        <div class="pond-secondary-column-content-region pond-column pond-secondary-column1 panel-panel span4">
          <div class="pond-secondary-column-content-region-inner pond-column-inner pond-secondary-column1-inner panel-panel-inner">
            <?php print $content['secondarycolumn1']; ?>
          </div>
        </div>
        <div class="pond-secondary-column-content-region pond-column pond-secondary-column2 panel-panel span4">
          <div class="pond-secondary-column-content-region-inner pond-column-inner pond-secondary-column2-inner panel-panel-inner">
            <?php print $content['secondarycolumn2']; ?>
          </div>
        </div>
        <div class="pond-secondary-column-content-region pond-column pond-secondary-column3 panel-panel span4">
          <div class="pond-secondary-column-content-region-inner pond-column-inner pond-secondary-column3-inner panel-panel-inner">
            <?php print $content['secondarycolumn3']; ?>
          </div>
        </div>
      </div>
    </div>
  </section>
    
  <footer class='section' id='footer' role='contentinfo'>
    <div class='container'>    
      <div class="pond-container pond-footer clearfix panel-panel">
        <div class="pond-container-inner pond-footer-inner panel-panel-inner">
          <?php print $content['footer']; ?>
        </div>
      </div>
    </div>
  </footer>
    
</div><!-- /.pond -->
