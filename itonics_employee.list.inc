<?php
/**
* Implements hook_form
* @see https://api.drupal.org/api/drupal/modules%21node%21node.api.php/function/hook_form/7.x
*/
function itonics_employee_list_form($form, $form_state){
	
	$form['add_employee'] = array(
	  '#type' => 'link',
	  '#title' => t('Add Employee link'),
	  '#href' => 'admin/employee/add/',
	);

	$results = db_select('employee','e')
	->extend('PagerDefault')
	->fields('e')
	->orderBy('e.id', 'DESC')
	->limit(2)
	->execute()
	->fetchAll();

    $header = array(
		'sn'      => array('data' => t('S.N'), 'field' => 'e.id'),
		'profile_picture' => t('Profile Picture'),
		'name'    => array('data' => t('Name'), 'field' => 'e.name'),
		'address' => array('data' => t('Address'), 'field' => 'e.address'),
		'email'   => array('data' => t('Email'), 'field' => 'e.email'),
		'gender'  => array('data' => t('Gender'), 'field' => 'e.gender'),
		'action'  => t('Action')
	);

    $options = array();
    foreach( $results as $i => $row ){

    	$img = theme('image', array('path' => $row->image));
		$options[$row->id] = array(
			'sn'      => $i+1,
			'profile_picture' => $img,
			'name'    => $row->name,
			'address' => $row->address,
			'email'   => $row->email,
			'gender'  => $row->gender,
			'action'  => array(
				'data' => array(
					array(
						'#type'  => 'link',
						'#title' => t('View / Edit'),
						'#href'  => 'admin/employee/edit/' . $row->id,
						'#suffix' => ' | '
					),
					array(
						'#type'  => 'link',
						'#title' => t('Delete'),
						'#href'  => 'admin/employee/delete' . $row->id,
					),
				)
			)
		);
    }

   	$form['nodes'] = array(
		'#type' => 'tableselect',
		'#header' => $header,
		'#options' => $options,
		'#empty' => t('No content available.'),
	);

	$form['pager'] = array('#markup' => theme('pager'));

	return $form;
}

function itonics_employee_list_form_submit($form, $form_state) {
}