<?php
/**
* Callback function for edit page
* Redirects to employee listing page if invalid argument is supplied
*
* @param $id
* @uses Itonics_Employee_Helper::menu
* @uses Itonics_Employee_Model::get_by_id
* @uses drupal_get_form
* @uses drupal_set_message
* @uses drupal_goto
* @uses Itonics_Employee_Helper::msg
* @return array || void
*/
function itonics_employee_edit($id=false){
	$path = Itonics_Employee_Helper::menu('list');
	if( $id && $id > 0 ){
		# Fetch edited row from employee table
		$edited_row = Itonics_Employee_Model::get_by_id($id);
		if( $edited_row ){
			$form = drupal_get_form('itonics_employee_add_edit_form', $edited_row);
			return $form;
		}else{
			drupal_set_message(Itonics_Employee_Helper::msg(4), 'error');
			drupal_goto($path);
		}
	}else{
		drupal_set_message(Itonics_Employee_Helper::msg(5), 'error');
		drupal_goto($path);
	}
}

/**
* Implements hook_form
*
* @see https://api.drupal.org/api/drupal/modules%21node%21node.api.php/function/hook_form/7.x
* @param $form, $form_state, $edited_row
* @uses t()
* @uses itonics_employee_get_address()
* @uses itonics_employee_get_gender()
* @return array
*/
function itonics_employee_add_edit_form($form, $form_state, $edited_row=false){
	if($edited_row){
		# Assign id of an edited employee show that we known if the operation is update or insertion
		# in 'itonics_employee_add_edit_form_submit' function
		$form['id'] = array('#type' => 'hidden', '#value' => $edited_row['id']);
		$form['old_file'] = array('#type' => 'hidden', '#value' => $edited_row['image']);

	}

	$def = function($key) use ($edited_row){
		return $edited_row ? $edited_row[$key] : '';
	};

	$form['name'] = array(
	  '#type'          => 'textfield', 
	  '#title'         => t('Name'), 
	  '#size'          => 60, 
	  '#maxlength'     => 128, 
	  '#required'      => TRUE,
	  '#default_value' =>  $def('name'), 
	);

	$form['email'] = array(
	  '#type'          => 'textfield', 
	  '#title'         => t('Email'),
	  '#required'      => TRUE,
	  '#default_value' => $def('email'), 
	);

	$form['address'] = array(
	    '#type'          => 'select',
	    '#title'         => t('Address'),
	    '#options'       => itonics_employee_get_address(),
	    '#description'   => t('Select your address.'),
	    '#default_value' => $def('address'), 
	);

	$form['gender'] = array(
		'#type'          => 'radios',
		'#title'         => t('Gender'),
		'#default_value' => 'male',
		'#options'       => itonics_employee_get_gender(),
		'#description'   => t('Select your gender.'),
		'#default_value' => $def('gender') == '' ? 'male' : $def('gender'), 
	);

	$form['projects'] = array(
		'#title'         => t('Projects'),
		'#type'          => 'textarea',
		'#description'   => t('Your Project details'),
		'#default_value' => $def('projects'), 
	);

	$form['additional_details'] = array(
		'#title'         => t('Additional Details'),
		'#type'          => 'text_format',
		'#description'   => t('Additional details'),
		'#default_value' => $def('additional_details'), 
	);

	$form['profile_picture'] = array(
	  '#title'           => t('Image'),
	  '#type'            => 'managed_file',
	  '#upload_location' => 'public://',
	  '#upload_validators' => array(
			'file_validate_size' => array('5242880'), // 5 MB
			'file_validate_extensions' => array('jpg png jpeg gif')
	   ),
	  '#default_value'   => $def('image'),
	);

    $form['submit'] = array(
       '#type'  => 'submit',
       '#value' => $edited_row ? t('Update') : t('Save'),
    );

	return $form;
}

/**
* Validate the inputs before submiting to the database
*
* @param $form, $form_state
* @uses filter_var()
* @uses form_set_error()
* @uses Itonics_Employee_Helper::msg()
* @return void
*/
function itonics_employee_add_edit_form_validate($form, &$form_state) {
	$email = $form_state['values']['email'];
	if( !filter_var($email, FILTER_VALIDATE_EMAIL) ){
		form_set_error('email', Itonics_Employee_Helper::msg(6));
	}

	# validate address field
	$address = itonics_employee_get_address();
	if(!in_array($form_state['values']['address'], array_keys($address))){
		form_set_error('address', Itonics_Employee_Helper::msg(10));
	}

	# validate address field
	$gender = itonics_employee_get_gender();
	if(!in_array($form_state['values']['gender'], array_keys($gender))){
		form_set_error('gender', Itonics_Employee_Helper::msg(11));
	}
	foreach(['name','email','projects'] as $v){
		$form_state['values'][$v] = check_plain( $form_state['values'][$v] );
	}

	$ai = $form_state['values']['additional_details']['value'];
	switch($form_state['values']['additional_details']['format']){
		case 'plain_text':
			$ai = check_plain($ai);
		break;

		case 'filtered_html':
			$ai = filter_xss($ai);
		break;

		case 'full_html':
			$ai = filter_xss_admin($ai);
		break;
	}
	
	$form_state['values']['additional_details']['value'] = $ai;
}

/**
* Submit inputs to the database
*
* @param $form, $form_state
* @uses file_load()
* @uses file_save()
* @uses file_delete()
* @uses file_usage_add()
* @uses drupal_set_message()
* @uses drupal_goto()
* @uses Itonics_Employee_Model::update()
* @uses Itonics_Employee_Helper::msg()
* @uses Itonics_Employee_Helper::save()
* @uses Itonics_Employee_Helper::menu()
* @return void
*/
function itonics_employee_add_edit_form_submit($form, $form_state) {

	$input = $form_state['values'];
	$file  = false;
	$id    = false;

	$fields = array(
		'name'     => $input['name'],
		'email'    => $input['email'],
		'address'  => $input['address'],
		'gender'   => $input['gender'],
		'projects' => $input['projects'],
		'image'    => '',
		'additional_details' => $input['additional_details']['value'],
	);

	if($input['profile_picture']){

		$file = file_load($input['profile_picture']);
		if($file){
			$fields['image'] = $file->fid;
			$file->status = FILE_STATUS_PERMANENT;
			file_save($file);
		}

		#delete old file from the database if exists
		if(isset($input['old_file']) && $input['old_file'] != $input['profile_picture']){
			$old_file = file_load($input['old_file']);
			if($old_file){
				file_delete($old_file,true);
			}
		}
	}

	if( isset($input['id']) ){
		# Update it
		$updated = Itonics_Employee_Model::update($fields, $input['id']);
		if($updated > 0){
			drupal_set_message(Itonics_Employee_Helper::msg(1));
		}else{
			drupal_set_message(Itonics_Employee_Helper::msg(3), 'warning');
		}

		$id = $input['id'];
	}else{
		# Insert it
		$id = Itonics_Employee_Model::save($fields);
		if($id){
			drupal_set_message(Itonics_Employee_Helper::msg(0));
			drupal_goto(Itonics_Employee_Helper::menu('list'));
		}else{
			drupal_set_message(Itonics_Employee_Helper::msg(2), 'error');
		}
	}

	if($id && $file){
		file_usage_add($file, 'itonics_employee', 'image', $id);
	}
}
