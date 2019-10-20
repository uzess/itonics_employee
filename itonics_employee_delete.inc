<?php
function itonics_employee_delete_confirm( $form, &$form_state, $id ){
	
	$path = Itonics_Employee_Helper::menu('list');
	$form_state['storage']['delete_id'] = $id;

	$content = '<h4>Confirmation dialog</h4>';

	$form['intro'] = array('#markup' => $content);

	$title = 'Confirmation';
	$description ="Do you really want to delete the employee?";
	$yes = "Ok";

	return confirm_form($form, $title , $path, $description , $yes);
	
}

function itonics_employee_delete_confirm_submit($form, &$form_state){
	$id = $form_state['storage']['delete_id'];
	$deleted = Itonics_Employee_Model::delete($id);
	if($deleted){
		drupal_set_message(Itonics_Employee_Helper::msg(7));
	}else{
		drupal_set_message(Itonics_Employee_Helper::msg(8),'warning');
	}
	$form_state['redirect'] = Itonics_Employee_Helper::menu('list');
  	return;
}