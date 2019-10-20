<section class="itonics-employee-view">
	<div class="itonics-employee-item">
		<span><?php echo t('Name:'); ?></span>
		<span><?php echo check_plain($name); ?></span>
	</div>
	<div class="itonics-employee-item">
		<span><?php echo t('Email:'); ?></span>
		<span><?php echo check_plain($email); ?></span>
	</div>	
	<div class="itonics-employee-item">
		<span><?php echo t('Gender:'); ?></span>
		<span><?php echo check_plain($gender); ?></span>
	</div>	
	<div class="itonics-employee-item">
		<span><?php echo t('Address:'); ?></span>
		<span><?php echo check_plain($address); ?></span>
	</div>	
	<div class="itonics-employee-item">
		<span><?php echo t('Projects:'); ?></span>
		<span><?php echo check_plain($projects); ?></span>
	</div>
	<div class="itonics-employee-item">
		<span><?php echo t('Profile Picture:'); ?></span>
		<span><?php echo $image; ?></span>
	</div>
	<div class="itonics-employee-item">
		<span><?php echo t('Additional Details:'); ?></span>
		<span><?php echo $additional_details; ?></span>
	</div>
</section>