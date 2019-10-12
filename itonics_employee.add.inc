<?php
/**
* Implements hook_form
* @see https://api.drupal.org/api/drupal/modules%21node%21node.api.php/function/hook_form/7.x
*/
function itonics_employee_add_form($form, $form_state, $id=false){
	$edited_row = false;
	if($id){

		$edited_row = db_select('employee','e')
		->fields('e')
		->condition('id', $id)
		->execute()
		->fetchAssoc();

		$form['id'] = array('#type' => 'hidden', '#value' => $id);
	}

	$def = function($key) use ($edited_row){
		return $edited_row ? $edited_row[$key] : '';
	};

	$form['name'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Name'), 
	  '#size' => 60, 
	  '#maxlength' => 128, 
	  '#default_value' =>  $def('name'), 
	  '#required' => TRUE,
	);

	$form['email'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Email'),
	  '#default_value' => $def('email'), 
	  '#required' => TRUE,
	);

	$form['address'] = array(
	    '#type' => 'select',
	    '#title' => t('Address'),
	    '#options' => array(
	       'nepal' => t('Nepal'),
	       'india' => t('India'),
	       'china' => t('China'),
	    ),
	    '#default_value' => $def('address'), 
	    '#description' => t('Select your address.'),
	);

	$form['gender'] = array(
		'#type' => 'radios',
		'#title' => t('Gender'),
		'#default_value' => 'male',
		'#options' => array(
			'male' => t('Male'), 
			'female' => t('Female'), 
			'other' => t('Other')
		),
		'#default_value' => $def('gender'), 
		'#description' => t('Select your gender.'),
	);

	$form['projects'] = array(
		'#title' => t('Projects'),
		'#type' => 'textarea',
		'#description' => t('Your Project details'),
		'#default_value' => $def('projects'), 
	);

	$form['additional_details'] = array(
		'#title' => t('Additional Details'),
		'#type' => 'text_format',
		'#description' => t('Additional details'),
		'#default_value' => $def('additional_details'), 
	);

	$form['profile_picture'] = array(
	    '#type' => 'file',
	    '#title' => t('Choose an image'),
	);

    $form['submit'] = array(
       '#type' => 'submit',
       '#value' => $edited_row ? t('Update') : t('Save'),
    );

	return $form;
}

function itonics_employee_add_form_validate($form, $form_state) {
	$email = $form_state['input']['email'];
	if( !filter_var($email, FILTER_VALIDATE_EMAIL) ){
		form_set_error('email', t('Invalid Email Address'));
	}

	$upload_validators = array(
		'file_validate_size' => array('5242880'), // 5 MB
		'file_validate_extensions' => array('jpg png jpeg gif')
	);

	$location = 'public://';
	$upload_check = file_save_upload('profile_picture', $upload_validators, $location);
	if($upload_check){
		$link = file_create_url($upload_check->uri);
		$_SESSION['uploaded_link'] = $link;
	}
}

function itonics_employee_add_form_submit($form, $form_state) {
	$table = 'employee';
	$input = $form_state['values'];
	$fields = array(
		'name'     => $input['name'],
		'email'    => $input['email'],
		'address'  => $input['address'],
		'gender'   => $input['gender'],
		'projects' => $input['projects'],
		'additional_details' => $input['additional_details']['value'],
	);

	if( isset($_SESSION['uploaded_link']) ){
		$fields['image'] = $_SESSION['uploaded_link'];
		unset($_SESSION['uploaded_link']);
	}

	if( isset($input['id']) ){
		# Update it
		$updated = db_update($table)
		->condition('id', $input['id'])
		->fields($fields)
		->execute();
		if($updated > 0){
			drupal_set_message(t('Employee Updated successfully.'));
		}else{
			drupal_set_message(t('No changes made.'), 'warning');
		}
	}else{
		# Insert it
		$id = db_insert($table)->fields($fields)->execute();
		if($id){
			drupal_set_message(t('Employee added successfully.'));
			drupal_goto('admin/employee');
		}else{
			drupal_set_message(t('Failed to add Employee.'), 'error');
		}
	}

}
