<?php
/**
* Helper functions for module
*
*/
class Itonics_Employee_Helper{

	public static function get_index($i){

		$get   = drupal_get_query_parameters();
		$page  = isset($get['page']) ? $get['page'] : 0;
		$limit = Itonics_Employee_Model::$limit;

		return ($page * $limit) + $i+1;
	}

	public static function menu( $slug ){

		$path = array(
			'list' => 'admin/employee',
			'add'  => 'admin/employee/add',
			'edit' => 'admin/employee/edit',
		);

		return isset($path[$slug]) ? $path[$slug] : '';
	}

	public static function msg($key){

		$msg = array(
			0 => t('Employee added successfully.'),
			1 => t('Employee Updated successfully.'),
			2 => t('Failed to add Employee.'),
			3 => t('No changes made.'),
			4 => t('Employee not found.'),
			5 => t('Invalid employee id.'),
			6 => t('Invalid Email Address.'),
			7 => t('Employee deleted successfully.'),
			8 => t('Nothing deleted.'),
			9 => t('Are you sure to delete?'),
			10 => t('Invalid address'),
			11 => t('Invalid gender'),
		);

		return isset($msg[$key]) ? $msg[$key] : t('Unknown message.');
	}
}