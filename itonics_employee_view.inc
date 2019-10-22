<?php
/**
* Implements hook_view
* @see https://api.drupal.org/api/drupal/modules%21system%21system.api.php/function/hook_view/7.x
* @uses Itonics_Employee_Model::get_by_id()
* @uses Itonics_Employee_Helper::menu()
* @uses image_style_url()
* @uses file_load()
* @uses drupal_goto()
* @param $id
* @return array | void
*/
function itonics_employee_view( $id=false ){ 
	if($id > 0){
		$row = Itonics_Employee_Model::get_by_id($id);
		if($row){

	    	$img = false;
	    	if(!empty($row['image'])){
	    		$file = file_load($row['image']);
	    		if($file){
		    		$img = '<img src="'. image_style_url('thumbnail', $file->uri) .'"/>';
	    		}
	    	}

	    	$output = array(
	    		'#theme' => 'itonics_employee_template',
	    		'#type'  => 'markup',
	    		'#image' => $img
	    	);

	    	$ignore = array('id','image');
	    	foreach( array_keys($row) as $key ){

	    		if( !in_array($key, $ignore)){
	    			$output['#'.$key] = $row[$key];
	    		}
	    	}
			return $output;
		}
	}else{
		drupal_goto(Itonics_Employee_Helper::menu('list'));
	}
}