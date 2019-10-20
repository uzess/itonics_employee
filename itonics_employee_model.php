<?php
/**
* Handles the database operation
*
*/
class Itonics_Employee_Model{

	public static $table = 'employee';

	public static $limit = 3;

	protected static $fields = 'e';

	public static function get($header){

		$result = db_select(self::$table,self::$fields)->extend('PagerDefault')->extend('TableSort')
			->fields(self::$fields)
			->orderByHeader($header)
			->limit(self::$limit)
			->execute()
			->fetchAll();

		return $result;
	}

	public static function get_by_id($id){
		$row = db_select(self::$table,self::$fields)
			->fields(self::$fields)
			->condition('id', $id)
			->execute()
			->fetchAssoc();

		return $row;
	}

	public static function save($fields, $pk = []){
		$id = drupal_write_record(self::$table, $fields, $pk);
		return $id;
	}

	public static function delete($id){
		$deleted = db_delete(self::$table)
			->condition('id', $id)
			->execute();
		if($id > 0){
			watchdog('itonics_employee', 'Employee ID %id has been deleted.', array(
	        	'%id' => $id,
	      	));
		}
		return $deleted;
	}
}