jQuery(document).ready(function(){
	jQuery("#itonics-employee-list-form").submit(function (e) { 
		return confirm(Drupal.settings.itonics_employee.confirmation);
	});
});