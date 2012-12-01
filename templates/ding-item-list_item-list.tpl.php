<?php
/**
 * @file
 *
 * Wrapper template for item list.
 */
?>

<div class="ding-item-list">
  <?php if (!empty($items)) : ?>
  <div class="pane-title"><?php echo t('Related materials'); ?></div>
  <div class="ding-item-list-items">
    <?php print $items; ?>
  </div>
  <?php ;endif ?>
</div>
