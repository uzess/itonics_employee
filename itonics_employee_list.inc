<?php
/**
* Implements hook_form
* @see https://api.drupal.org/api/drupal/modules%21node%21node.api.php/function/hook_form/7.x
* @param $form, $form_state
* @uses t()
* @uses file_load()
* @uses file_create_url()
* @uses image_style_url()
* @uses theme()
* @uses Itonics_Employee_Model::get()
* @uses Itonics_Employee_Helper::menu()
* @return array
*/
function itonics_employee_list_form($form, &$form_state){
	
	$path = current_path();
	if(isset( $form_state['storage']['confirm'])){

		$content = '<h4>Confirmation dialog</h4>';

		$form['intro'] = array('#markup' => $content);

		$title = 'Confirmation';
		$description ="Do you really want to delete the employee?";
		$yes = "Ok";

		return confirm_form($form, $title , $path, $description , $yes);
		
	}else{

		$form['add_employee'] = array(
		  '#type'  => 'link',
		  '#title' => t('Add Employee link'),
		  '#href'  => Itonics_Employee_Helper::menu('add'),
		);

	    $header = array(
			'sn'      => array('data' => t('S.N'), 'field' => 'e.id', 'sort' => 'DESC'),
			'profile_picture' => t('Profile Picture'),
			'name'    => array('data' => t('Name'), 'field' => 'e.name'),
			'address' => array('data' => t('Address'), 'field' => 'e.address'),
			'email'   => array('data' => t('Email'), 'field' => 'e.email'),
			'gender'  => array('data' => t('Gender'), 'field' => 'e.gender'),
			'action'  => t('Action')
		);

		$results = Itonics_Employee_Model::get($header);
	    $options = array();
	    foreach( $results as $i => $row ){

	    	$img = t('No image');
	    	if(!empty($row->image)){
	    		$file = file_load($row->image);
	    		if($file){
		    		$img = '<img src="'. image_style_url('thumbnail', $file->uri) .'"/>';
	    		}
	    	}

			$options[$row->id] = array(
				'sn'      => Itonics_Employee_Helper::get_index($i),
				'profile_picture' => $img,
				'name'    => check_plain($row->name),
				'address' => ucfirst(check_plain($row->address)),
				'email'   => check_plain($row->email),
				'gender'  => ucfirst(check_plain($row->gender)),
				'action'  => array(
					'data' => array(
						array(
							'#type'  => 'link',
							'#title' => t('edit'),
							'#href'  => Itonics_Employee_Helper::menu('edit') . '/' . $row->id,
							'#suffix' => ' &nbsp; '
						),
						array(
							'#type' => 'link',
							'#title' => t('delete'),
							'#href'  => Itonics_Employee_Helper::menu('delete') . '/' . $row->id,
							'#suffix' => ' &nbsp; '
						),
						array(
							'#type' => 'link',
							'#title' => t('view'),
							'#href'  => Itonics_Employee_Helper::menu('view') . '/' . $row->id,
						)
					)
				)
			);
	    }

	   	$form['nodes'] = array(
			'#type'    => 'tableselect',
			'#header'  => $header,
			'#options' => $options,
			'#empty'   => t('No content available.'),
		);

	   	if(count($results) > 0){
			$form['confirm'] = array(
			   '#type'  => 'submit',
			   '#value' => t('Delete'),
			);

			$form['pager'] = array('#markup' => theme('pager'));
	   	}
	}

	return $form;
}

/**
* Handle list form submission
*
* @param $form, $form_state
* @uses Itonics_Employee_Model::get_by_id()
* @uses Itonics_Employee_Model::delete()
* @uses Itonics_Employee_Helper::msg()
* @uses file_load()
* @uses file_delete()
* @uses drupal_set_message()
* @return void
*/
function itonics_employee_list_form_submit($form, &$form_state) {
	
	if( !isset($form_state['storage']['confirm'] )){
		$form_state['storage']['confirm'] = TRUE;
		$form_state['storage']['nodes'] = $form_state['values']['nodes'];
		$form_state['rebuild'] = TRUE;
	}else{
		$nodes = $form_state['storage']['nodes'];
		$deleted = false;
		foreach($nodes as $key => $id){
			if($id > 0){
				$row = Itonics_Employee_Model::get_by_id($id);
				if($row){
					$deleted = Itonics_Employee_Model::delete($id);
					if($row['image'] > 0){
						$file = file_load($row['image']);
						if($file){
							file_delete($file, true);
						}
					}
				}
			}
		}

		if($deleted){
			drupal_set_message(Itonics_Employee_Helper::msg(7));
		}else{
			drupal_set_message(Itonics_Employee_Helper::msg(8),'warning');
		}
	}
	
}