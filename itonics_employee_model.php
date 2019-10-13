<?php
/**
* Handles the database operation
*
*/
class Itonics_Employee_Model{

	public static $table = 'employee';

	protected static $limit = 20;

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

	public static function update($fields, $id){
		$updated = db_update(self::$table)
			->condition('id', $id)
			->fields($fields)
			->execute();

		if($updated){
			watchdog('itonics_employee', 'Employee ID %id has been updated.', array(
	        	'%id' => $id,
	      	));	
		}	
		return $updated;
	}

	public static function save($fields){
		$id = db_insert(self::$table)->fields($fields)->execute();
		if($id > 0){
			watchdog('itonics_employee', 'Employee ID %id has been inserted.', array(
	        	'%id' => $id,
	      	));
		}
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