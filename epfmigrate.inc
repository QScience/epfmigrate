<?php

class epfuserMigration extends Migration {
  public function __construct() {
    parent::__construct();
    // The example uses a SQL database as source.
	
	$this->map = new MigrateSQLMap($this->machineName,
        array('id' => array(
                'type' => 'varchar',
                'length' => 255,
                 'not null' => TRUE,
                'description' => 'Account name.'
                )
             ),
        MigrateDestinationUser::getKeySchema()
    );
	$query = Database::getConnection('default','for_migration')
           ->select('users', 'b')
             ->fields('b', array('id','password','email','firstName','lastName','affiliation','homepage','registerTime','lastLoginTime'));
    $this->source = new MigrateSourceSQL($query);

    // We migrate into user.
    $user_options = MigrateDestinationUser::options(LANGUAGE_NONE, 'filtered_html', true);
	$this->destination = new MigrateDestinationUser($user_options);
    // Finally we add simple field mappings 
    $this->addFieldMapping('name', 'id');
    $this->addFieldMapping('mail', 'email');
    $this->addFieldMapping('pass', 'password');
    $this->addFieldMapping('created', 'registerTime');
    $this->addFieldMapping('login', 'lastLoginTime');
    $this->addFieldMapping('first_name', 'firstName');
    $this->addFieldMapping('last_name', 'lastName');
    $this->addFieldMapping('field_institution', 'affiliation');
    $this->addFieldMapping('website', 'homepage');
    $this->addFieldMapping('status')->defaultValue(1);
  }
}

class bookMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser');
    $this->map = new MigrateSQLMap($this->machineName,
        array('id' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                 'not null' => TRUE,
                'description' => 'book id.'
                )
             ),
        MigrateDestinationNode::getKeySchema()
    );
    // The example uses a SQL database as source.
     $query = Database::getConnection('default', 'for_migration')
           ->select('books', 'b')
             ->fields('b', array('id','title','authors','abstract','reviewer','content',	'reviewNum','voteNum','postUser','updateTime','verified','removed'));
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $node_options = MigrateDestinationNode::options(LANGUAGE_NONE, 'full_html');
    $this->destination = new MigrateDestinationNode('bookreview',$node_options);


    // Finally we add simple field mappings 
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('authorname', 'authors');
    $this->addFieldMapping('field_reviewer', 'reviewer');
    $this->addFieldMapping('abstract', 'content');
    $this->addFieldMapping('uid', 'postUser')
         ->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('created', 'updateTime');
    $this->addFieldMapping('status')->defaultValue(1);
  }
  public function prepareRow($row) {
  // If marked as removed or not verified in the related data, skip this row
  if ($row->removed==1||$row->verified==0) {
    return FALSE;
  }
  }
}
class editorialMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser');
    $this->map = new MigrateSQLMap($this->machineName,
        array('id' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                 'not null' => TRUE,
                'description' => 'editorial id.'
                )
             ),
        MigrateDestinationNode::getKeySchema()
    );
    // The example uses a SQL database as source.
     $query = Database::getConnection('default', 'for_migration')
           ->select('editorials', 'b')
             ->fields('b', array('id','title','authors','abstract','content','reviewNum','voteNum','postUser','updateTime','verified','removed'));
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $node_options = MigrateDestinationNode::options(LANGUAGE_NONE, 'full_html');
    $this->destination = new MigrateDestinationNode('editorial',$node_options);


    // Finally we add simple field mappings 
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('authorname', 'authors');
    $this->addFieldMapping('abstract', 'abstract');
    $this->addFieldMapping('note', 'content');
    $this->addFieldMapping('uid', 'postUser')
         ->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('created', 'updateTime');
    $this->addFieldMapping('status','verified');
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if ($row->removed==1) {
    return FALSE;
  }
  $row->title=strip_tags($row->title);
  }
}
class eventMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser');
    $this->map = new MigrateSQLMap($this->machineName,
        array('id' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                 'not null' => TRUE,
                'description' => 'event id.'
                )
             ),
        MigrateDestinationNode::getKeySchema()
    );
    // The example uses a SQL database as source.
     $query = Database::getConnection('default', 'for_migration')
           ->select('events', 'b')
             ->fields('b', array('id','title','link','fromTime','toTime','location','reviewNum','voteNum','postUser','updateTime','verified','removed'));
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $node_options = MigrateDestinationNode::options(LANGUAGE_NONE, 'full_html');
    $this->destination = new MigrateDestinationNode('event',$node_options);


    // Finally we add simple field mappings 
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('datebegin', 'fromTimedate');
    $this->addFieldMapping('dateend', 'toTimedate');
    $this->addFieldMapping('city', 'location');
    $this->addFieldMapping('website', 'link');
    $this->addFieldMapping('uid', 'postUser')
         ->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('created', 'updateTime');
    $this->addFieldMapping('status','verified');
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if ($row->removed==1) {
    return FALSE;
  }
  //change the formate of fromTime and toTime
    $date_data = array(
      'from' =>  $row->fromTime,
    );
    $row->fromTimedate=drupal_json_encode($date_data);
    $date_data = array(
      'from' => $row->toTime,
    );
    $row->toTimedate=drupal_json_encode($date_data);
  //  $row->toTime=date("Y-m-d H:i:s",$row->toTime);
  }
}
class newsMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser');
    $this->map = new MigrateSQLMap($this->machineName,
        array('id' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                 'not null' => TRUE,
                'description' => 'news id.'
                )
             ),
        MigrateDestinationNode::getKeySchema()
    );
    // The example uses a SQL database as source.
     $query = Database::getConnection('default', 'for_migration')
           ->select('news', 'b')
             ->fields('b', array('id','prefix','title','abstract','content','isLink','reviewNum','voteNum','postUser','updateTime','verified','removed'));
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $node_options = MigrateDestinationNode::options(LANGUAGE_NONE, 'full_html');
    $this->destination = new MigrateDestinationNode('news',$node_options);


    // Finally we add simple field mappings 
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('field_prefix', 'prefix');
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('website', 'link');
    $this->addFieldMapping('uid', 'postUser')
         ->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('created', 'updateTime');
    $this->addFieldMapping('status','verified');
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if ($row->removed==1||$row->prefix==='Job Opening') {
    return FALSE;
  }
  if($row->isLink){
  	$row->link=$row->content;
  	$row->body=$row->abstract;
  }else{
  	$row->link='';
  	$row->body=$row->content;
  }
  }
}
class jobsMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser');
    $this->map = new MigrateSQLMap($this->machineName,
        array('id' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                 'not null' => TRUE,
                'description' => 'jobs id.'
                )
             ),
        MigrateDestinationNode::getKeySchema()
    );
    // The example uses a SQL database as source.
     $query = Database::getConnection('default', 'for_migration')
           ->select('news', 'b')
             ->fields('b', array('id','prefix','title','abstract','content','isLink','reviewNum','voteNum','postUser','updateTime','verified','removed'))
             ->condition('prefix','Job Opening');
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $node_options = MigrateDestinationNode::options(LANGUAGE_NONE, 'full_html');
    $this->destination = new MigrateDestinationNode('job_opening',$node_options);


    // Finally we add simple field mappings 
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('website', 'link');
    $this->addFieldMapping('uid', 'postUser')
         ->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('created', 'updateTime');
    $this->addFieldMapping('status','verified');
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if ($row->removed==1) {
    return FALSE;
  }
  if($row->isLink){
  	$row->link=$row->content;
  	$row->body=$row->abstract;
  }else{
  	$row->link='';
  	$row->body=$row->content;
  }
  }
}
class papersArxivMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser');
    $this->map = new MigrateSQLMap($this->machineName,
        array('id' => array(
                'type' => 'varchar',
                'length' => 30,
                 'not null' => TRUE,
                'description' => 'papers id.'
                )
             ),
        MigrateDestinationNode::getKeySchema()
    );
    // The example uses a SQL database as source.
     $query = Database::getConnection('default', 'for_migration')
           ->select('papers', 'b')
             ->fields('b', array('id','title','abstract','authors','reviewNum','downloadNum','voteNum','pdfUrl','psUrl','otherUrl','postUser','postTime','verified','removed'))
             ->condition('removed',0);
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $node_options = MigrateDestinationNode::options(LANGUAGE_NONE, 'full_html');
    $this->destination = new MigrateDestinationNode('paper_from_arxiv',$node_options);


    // Finally we add simple field mappings 
    $this->addFieldMapping('title', 'title');   
    $this->addFieldMapping('field_serial_no', 'paperinfo');
 /*   $this->addFieldMapping('field_serial_no:serialNo', 'serialNo');
    $this->addFieldMapping('field_serial_no:authors', 'authors');
    $this->addFieldMapping('field_serial_no:abstract', 'abstract');
    $this->addFieldMapping('field_serial_no:pdfUrl', 'pdfUrl');
    $this->addFieldMapping('field_serial_no:psUrl', 'psUrl');
    $this->addFieldMapping('field_serial_no:otherUrl', 'otherUrl');
    $this->addFieldMapping('field_serial_no:downloadNo', 'downloadNo');*/
    $this->addFieldMapping('uid', 'postUser')
         ->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('created', 'postTime');
    $this->addFieldMapping('status','verified');
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if ($row->removed==1) {
    return FALSE;
  }
  if(substr($row->id,1)>'0'&&substr($row->id,1)<='9'){
  	 $serialNo=$row->id;
  }else{
  	$serialNo='';
  }
 
  $row->serialNo=$serialNo;
  $row->paperinfo= array(
    'serialNo'=>$serialNo,		 	 	 	 	 	 	
	'authors'=>$row->authors,		 	 	 	 	 	 	
	'abstract'=>$row->abstract,	 	 	 	 	 	 	
	'pdfUrl'=>$row->pdfUrl,	 	 	 	 	 	 	
	'psUrl'=>$row->psUrl,	 	 	 	 	 	 	
	'otherUrl'=>$row->otherUrl,		 	 	 	 	 	 	
	'downloadNo'=>$row->downloadNum,
  );
  }
}
class commentbookMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser','book');
  	$this->map = new MigrateSQLMap($this->machineName,
        array('id' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                 'not null' => TRUE,
                'description' => 'comment id.',
                'alias' => 'b',
                )
             ),
        MigrateDestinationComment::getKeySchema()
    );
    // The example uses a SQL database as source.
     $query = Database::getConnection('default', 'for_migration')
           ->select('comments', 'b')
             ->fields('b', array('id','reId','category','content','postUser','postUserName','postUserEmail','postTime','removed'));
     $query ->leftJoin('users', 'ub', 'b.postUser = ub.id');
     $query->fields('ub',array('email'));
    $this->source = new MigrateSourceSQL($query);
    // We migrate into comments.
  //  $node_options = MigrateDestinationNode::options(LANGUAGE_NONE, 'full_html');
    $this->destination = new MigrateDestinationComment('comment_node_bookreview');


    // Finally we add simple field mappings 
    $this->addFieldMapping('nid', 'reId')->sourceMigration('book');
    $this->addFieldMapping('subject', 'subject');  
    $this->addFieldMapping('uid', 'postUser')
         ->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('name', 'postUserName');
    $this->addFieldMapping('mail', 'postUserEmail');
    $this->addFieldMapping('created', 'postTime');
    $this->addFieldMapping('status')->defaultValue(1);
    $this->addFieldMapping('comment_body', 'content');
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if ($row->removed==1||$row->category!="Book reviews") {
    return FALSE;
  }
  $row->subject=substr($row->content,0,60);
    if($row->postUser!='guest'){
  $row->postUserName=$row->postUser;
  	$row->postUserEmail=$row->email;
  }
  }
}
class commenteditorialMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser','editorial');
  	$this->map = new MigrateSQLMap($this->machineName,
        array('id' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                 'not null' => TRUE,
                'description' => 'comment id.',
                'alias' => 'b',
                )
             ),
        MigrateDestinationComment::getKeySchema()
    );
    // The example uses a SQL database as source.
     $query = Database::getConnection('default', 'for_migration')
           ->select('comments', 'b')
             ->fields('b', array('id','reId','category','content','postUser','postUserName','postUserEmail','postTime','removed'));
     $query ->leftJoin('users', 'ub', 'b.postUser = ub.id');
     $query->fields('ub',array('email'));
    $this->source = new MigrateSourceSQL($query);
    // We migrate into comments.
    $node_options = MigrateDestinationNode::options(LANGUAGE_NONE, 'full_html');
    $this->destination = new MigrateDestinationComment('comment_node_editorial',$node_options);


    // Finally we add simple field mappings 
    $this->addFieldMapping('nid', 'reId')->sourceMigration('editorial');
    $this->addFieldMapping('subject', 'subject');
    $this->addFieldMapping('comment_body', 'content');
    $this->addFieldMapping('uid', 'postUser')
         ->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('name', 'postUserName');
    $this->addFieldMapping('mail', 'postUserEmail');
    $this->addFieldMapping('created', 'postTime');
    $this->addFieldMapping('status')->defaultValue(1);
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if ($row->removed==1||$row->category!="Editorial") {
    return FALSE;
  }
  $row->subject=substr($row->content,0,60);
    if($row->postUser!='guest'){
  $row->postUserName=$row->postUser;
  	$row->postUserEmail=$row->email;
  }
  }
}
class commentnewsMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser','news');
  	$this->map = new MigrateSQLMap($this->machineName,
        array('id' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                 'not null' => TRUE,
                'description' => 'comment id.',
                'alias' => 'b',
                )
             ),
        MigrateDestinationComment::getKeySchema()
    );
    // The example uses a SQL database as source.
     $query = Database::getConnection('default', 'for_migration')
           ->select('comments', 'b')
             ->fields('b', array('id','reId','category','content','postUser','postUserName','postUserEmail','postTime','removed'));
     $query ->leftJoin('users', 'ub', 'b.postUser = ub.id');
     $query->fields('ub',array('email'));
    $this->source = new MigrateSourceSQL($query);
    // We migrate into comments.
    $node_options = MigrateDestinationNode::options(LANGUAGE_NONE, 'full_html');
    $this->destination = new MigrateDestinationComment('comment_node_news',$node_options);


    // Finally we add simple field mappings 
    $this->addFieldMapping('nid', 'reId')->sourceMigration('news');
    $this->addFieldMapping('subject', 'subject');
    $this->addFieldMapping('comment_body', 'content');
    $this->addFieldMapping('uid', 'postUser')
         ->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('name', 'postUserName');
    $this->addFieldMapping('mail', 'postUserEmail');
    $this->addFieldMapping('created', 'postTime');
    $this->addFieldMapping('status')->defaultValue(1);
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if ($row->removed==1||$row->category!="News") {
    return FALSE;
  }
  $row->subject=substr($row->content,0,60);
   if($row->postUser!='guest'){
  $row->postUserName=$row->postUser;
  	$row->postUserEmail=$row->email;
  }
  }
}
class commentjobsMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser','jobs');
  	$this->map = new MigrateSQLMap($this->machineName,
        array('id' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                 'not null' => TRUE,
                'description' => 'comment id.',
                'alias' => 'b',
                )
             ),
        MigrateDestinationComment::getKeySchema()
    );
    // The example uses a SQL database as source.
     $query = Database::getConnection('default', 'for_migration')
           ->select('comments', 'b')
             ->fields('b', array('id','reId','category','content','postUser','postUserName','postUserEmail','postTime','removed'));
     $query ->leftJoin('users', 'ub', 'b.postUser = ub.id');
     $query->fields('ub',array('email'));
    $this->source = new MigrateSourceSQL($query);
    // We migrate into comments.
    $node_options = MigrateDestinationNode::options(LANGUAGE_NONE, 'full_html');
    $this->destination = new MigrateDestinationComment('comment_node_job_opening',$node_options);


    // Finally we add simple field mappings 
    $this->addFieldMapping('nid', 'reId')->sourceMigration('jobs');
    $this->addFieldMapping('subject', 'subject');
    $this->addFieldMapping('comment_body', 'content');
    $this->addFieldMapping('uid', 'postUser')
         ->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('name', 'postUserName');
    $this->addFieldMapping('mail', 'postUserEmail');
    $this->addFieldMapping('created', 'postTime');
    $this->addFieldMapping('status')->defaultValue(1);
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if ($row->removed==1||$row->category!="News") {
    return FALSE;
  }
  $row->subject=substr($row->content,0,60);
   if($row->postUser!='guest'){
  $row->postUserName=$row->postUser;
  	$row->postUserEmail=$row->email;
  }
  }
}
class commentpapersArxivMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser','papersArxiv');
  	$this->map = new MigrateSQLMap($this->machineName,
        array('id' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                 'not null' => TRUE,
                'description' => 'comment id.',
                'alias' => 'b',
                )
             ),
        MigrateDestinationComment::getKeySchema()
    );
    // The example uses a SQL database as source.
     $query = Database::getConnection('default', 'for_migration')
           ->select('comments', 'b')
             ->fields('b', array('id','reId','category','content','postUser','postUserName','postUserEmail','postTime','removed'));
    $query ->leftJoin('users', 'ub', 'b.postUser = ub.id');
     $query->fields('ub',array('email'));
    $this->source = new MigrateSourceSQL($query);
    // We migrate into comments.
    $node_options = MigrateDestinationNode::options(LANGUAGE_NONE, 'full_html');
    $this->destination = new MigrateDestinationComment('comment_node_paper_from_arxiv',$node_options);


    // Finally we add simple field mappings 
    $this->addFieldMapping('nid', 'reId')->sourceMigration('papersArxiv');
    $this->addFieldMapping('subject', 'subject');
    $this->addFieldMapping('comment_body', 'content');
    $this->addFieldMapping('uid', 'postUser')
         ->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('name', 'postUserName');
    $this->addFieldMapping('mail', 'postUserEmail');
    $this->addFieldMapping('created', 'postTime');
    $this->addFieldMapping('status')->defaultValue(1);
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if ($row->removed==1||$row->category!="Papers") {
    return FALSE;
  }
  $row->subject=substr($row->content,0,60);
  if($row->postUser!='guest'){
  $row->postUserName=$row->postUser;
  	$row->postUserEmail=$row->email;
  }
  }
}
class blogMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser');
    $this->map = new MigrateSQLMap($this->machineName,
        array('ID' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                 'not null' => TRUE,
                'description' => 'news id.',
                'alias' => 'b',
                )
             ),
        MigrateDestinationNode::getKeySchema()
    );
    // The example uses a SQL database as source.
     $query = Database::getConnection('default', 'for_migration')
           ->select('wp_posts', 'b')
             ->fields('b', array('ID','post_date','post_content','post_title','post_status','post_type'));
     $query ->leftJoin('wp_users', 'ub', 'b.post_author = ub.ID');
     $query->fields('ub',array('user_login'));
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $node_options = MigrateDestinationNode::options(LANGUAGE_NONE, 'full_html');
    $this->destination = new MigrateDestinationNode('blog',$node_options);


    // Finally we add simple field mappings 
    $this->addFieldMapping('title', 'post_title');
    $this->addFieldMapping('body', 'post_content');
    $this->addFieldMapping('uid', 'user_login')
         ->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('created', 'post_date');
    $this->addFieldMapping('status')->defaultValue(1);
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if ($row->post_status!="publish"||$row->post_type!="post") {
    return FALSE;
  }
  
  }
}
class commentblogMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser','blog');
  	$this->map = new MigrateSQLMap($this->machineName,
        array('comment_ID' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                 'not null' => TRUE,
                'description' => 'comment id.'
                )
             ),
        MigrateDestinationComment::getKeySchema()
    );
    // The example uses a SQL database as source.
     $query = Database::getConnection('default', 'for_migration')
           ->select('wp_comments', 'b')
             ->fields('b', array('comment_ID','comment_post_ID','comment_author','comment_author_email','comment_date','comment_content','comment_approved','user_id'));
     $query ->leftJoin('wp_users', 'ub', 'b.user_id = ub.ID');
     $query->fields('ub',array('user_login','user_email'));
    $this->source = new MigrateSourceSQL($query);
    // We migrate into comments.
    $node_options = MigrateDestinationNode::options(LANGUAGE_NONE, 'full_html');
    $this->destination = new MigrateDestinationComment('comment_node_blog',$node_options);


    // Finally we add simple field mappings 
    $this->addFieldMapping('nid', 'comment_post_ID')->sourceMigration('blog');
    $this->addFieldMapping('comment_body', 'comment_content');
        $this->addFieldMapping('subject', 'subject');
    $this->addFieldMapping('uid', 'user_login')
         ->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('name', 'postUserName');
    $this->addFieldMapping('mail', 'postUserEmail');
    $this->addFieldMapping('created', 'comment_date');
    $this->addFieldMapping('status')->defaultValue(1);
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if ($row->comment_approved!=1) {
    return FALSE;
  }
   $row->subject=substr(strip_tags($row->comment_content),0,60);
  if($row->user_id==0){
  	$row->postUserName=$row->comment_author;
  	$row->postUserEmail=$row->comment_author_email;
  }else{
  	$row->postUserName=$row->user_login;
  	$row->postUserEmail=$row->user_email;
  }
  }
}

class voteMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser','papersArxiv');
    // The example uses a SQL database as source.
    $query =  Database::getConnection('default', 'for_migration')
             ->select('votes', 'p')
             ->fields('p', array('id','voteId','category','score','voteUser','voteTime','ip'))
             ->condition('voteUser','guest','!=')->condition('score',10);
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $this->destination = new MigrateDestinationVotingapi();

    // We instantiate the MigrateMap
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'id' => array('type' => 'int',
                          'unsigned' => TRUE,
                          'not null' => TRUE,
                         )
        ),
        MigrateDestinationNode::getKeySchema()
      );

    // Finally we add simple field mappings 
    $this->addFieldMapping('entity_type', 'type');
    $this->addFieldMapping('entity_id', 'voteId')->sourceMigration('papersArxiv')->defaultValue(1);
    $this->addFieldMapping('value_type', 'value_type');
    $this->addFieldMapping('value')->defaultValue(1);
    $this->addFieldMapping('uid', 'voteUser')->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('tag', 'tag');
    $this->addFieldMapping('timestamp', 'voteTime');
    $this->addFieldMapping('vote_source', 'ip');
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if($row->score<10){
  	return false;
  }
  $row->type="paper_from_arxiv";
  $row->value_type="points";
  $row->tag="plus1_node_vote";
  }
}

//update qtr_actions
class actionspapersMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser','papersArxiv');
      $query =  Database::getConnection('default', 'for_migration')
             ->select('votes', 'p')
             ->fields('p', array('id','voteId','category','score','voteUser','voteTime','ip'))
             ->condition('voteUser','guest','!=')->condition('category','Papers');
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $this->destination = new MigrateDestinationTable('qtr_actions');

    // We instantiate the MigrateMap
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'id' => array('type' => 'int',
                          'unsigned' => TRUE,
                          'not null' => TRUE,
                         )
        ),
        MigrateDestinationNode::getKeySchema()
      );

    // Finally we add simple field mappings 
    $this->addFieldMapping('nid', 'voteId')->sourceMigration('papersArxiv')->defaultValue(1);
    $this->addFieldMapping('action_type','typescore');
    $this->addFieldMapping('uid', 'voteUser')->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('timestamp', 'voteTime');
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if($row->score==1){
  	$row->typescore=3;
  }else if($row->score==3){
  	$row->typescore=2;
  }else if($row->score==10){
  	$row->typescore=1;
  }
  $row->voteTime = Migration::timestamp($row->voteTime);

  }
}
class actionsbookMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser','book');
      $query =  Database::getConnection('default', 'for_migration')
             ->select('votes', 'p')
             ->fields('p', array('id','voteId','category','score','voteUser','voteTime','ip'))
             ->condition('voteUser','guest','!=')->condition('category','Book reviews');
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $this->destination = new MigrateDestinationTable('qtr_actions');

    // We instantiate the MigrateMap
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'id' => array('type' => 'int',
                          'unsigned' => TRUE,
                          'not null' => TRUE,
                         )
        ),
        MigrateDestinationNode::getKeySchema()
      );

    // Finally we add simple field mappings 
    $this->addFieldMapping('nid', 'voteId')->sourceMigration('book')->defaultValue(1);
    $this->addFieldMapping('action_type','typescore');
    $this->addFieldMapping('uid', 'voteUser')->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('timestamp', 'voteTime');
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if($row->score==1){
  	$row->typescore=3;
  }else if($row->score==3){
  	$row->typescore=2;
  }else if($row->score==10){
  	$row->typescore=1;
  }
$row->voteTime = Migration::timestamp($row->voteTime);
  }
}
class actionseditorialMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser','editorial');
      $query =  Database::getConnection('default', 'for_migration')
             ->select('votes', 'p')
             ->fields('p', array('id','voteId','category','score','voteUser','voteTime','ip'))
             ->condition('voteUser','guest','!=')->condition('category','Editorial');
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $this->destination = new MigrateDestinationTable('qtr_actions');

    // We instantiate the MigrateMap
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'id' => array('type' => 'int',
                          'unsigned' => TRUE,
                          'not null' => TRUE,
                         )
        ),
        MigrateDestinationNode::getKeySchema()
      );

    // Finally we add simple field mappings 
    $this->addFieldMapping('nid', 'voteId')->sourceMigration('editorial')->defaultValue(1);
    $this->addFieldMapping('action_type','typescore');
    $this->addFieldMapping('uid', 'voteUser')->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('timestamp', 'voteTime');
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if($row->score==1){
  	$row->typescore=3;
  }else if($row->score==3){
  	$row->typescore=2;
  }else if($row->score==10){
  	$row->typescore=1;
  }
$row->voteTime = Migration::timestamp($row->voteTime);
  }
}
class actionseventMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser','event');
      $query =  Database::getConnection('default', 'for_migration')
             ->select('votes', 'p')
             ->fields('p', array('id','voteId','category','score','voteUser','voteTime','ip'))
             ->condition('voteUser','guest','!=')->condition('category','Events');
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $this->destination = new MigrateDestinationTable('qtr_actions');

    // We instantiate the MigrateMap
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'id' => array('type' => 'int',
                          'unsigned' => TRUE,
                          'not null' => TRUE,
                         )
        ),
        MigrateDestinationNode::getKeySchema()
      );

    // Finally we add simple field mappings 
    $this->addFieldMapping('nid', 'voteId')->sourceMigration('event')->defaultValue(1);
    $this->addFieldMapping('action_type','typescore');
    $this->addFieldMapping('uid', 'voteUser')->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('timestamp', 'voteTime');
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if($row->score==1){
  	$row->typescore=3;
  }else if($row->score==3){
  	$row->typescore=2;
  }else if($row->score==10){
  	$row->typescore=1;
  }
$row->voteTime = Migration::timestamp($row->voteTime);
  }
}
class actionsnewsMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser','news');
      $query =  Database::getConnection('default', 'for_migration')
             ->select('votes', 'p')
             ->fields('p', array('id','voteId','category','score','voteUser','voteTime','ip'))
             ->condition('voteUser','guest','!=')->condition('category','News');
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $this->destination = new MigrateDestinationTable('qtr_actions');

    // We instantiate the MigrateMap
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'id' => array('type' => 'int',
                          'unsigned' => TRUE,
                          'not null' => TRUE,
                         )
        ),
        MigrateDestinationNode::getKeySchema()
      );

    // Finally we add simple field mappings 
    $this->addFieldMapping('nid', 'voteId')->sourceMigration('news')->defaultValue(1);
    $this->addFieldMapping('action_type','typescore');
    $this->addFieldMapping('uid', 'voteUser')->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('timestamp', 'voteTime');
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if($row->score==1){
  	$row->typescore=3;
  }else if($row->score==3){
  	$row->typescore=2;
  }else if($row->score==10){
  	$row->typescore=1;
  }
$row->voteTime = Migration::timestamp($row->voteTime);
  }
}
class actionsjobsMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array('epfuser','jobs');
      $query =  Database::getConnection('default', 'for_migration')
             ->select('votes', 'p')
             ->fields('p', array('id','voteId','category','score','voteUser','voteTime','ip'))
             ->condition('voteUser','guest','!=')->condition('category','News');
    $this->source = new MigrateSourceSQL($query);
    // We migrate into "page" nodes.
    $this->destination = new MigrateDestinationTable('qtr_actions');

    // We instantiate the MigrateMap
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'id' => array('type' => 'int',
                          'unsigned' => TRUE,
                          'not null' => TRUE,
                         )
        ),
        MigrateDestinationNode::getKeySchema()
      );

    // Finally we add simple field mappings 
    $this->addFieldMapping('nid', 'voteId')->sourceMigration('jobs')->defaultValue(1);
    $this->addFieldMapping('action_type','typescore');
    $this->addFieldMapping('uid', 'voteUser')->sourceMigration('epfuser')
         ->defaultValue(0);
    $this->addFieldMapping('timestamp', 'voteTime');
  }
  public function prepareRow($row) {
  // If marked as removed in the related data, skip this row
  if($row->score==1){
  	$row->typescore=3;
  }else if($row->score==3){
  	$row->typescore=2;
  }else if($row->score==10){
  	$row->typescore=1;
  }
$row->voteTime = Migration::timestamp($row->voteTime);
  }
}
class ArxivMigrateFieldHandler extends MigrateFieldHandler {

  /**
   * Declares the types of fields used.
   */
  public function __construct() {
    $this->registerTypes(array('arxiv'));
  }

  /**
   * Converts incoming data into the proper field arrays for arxiv fields.
   *
   * @param object $entity
   *   The destination entity which will hold the field arrays.
   * @param array $field_info
   *   Metadata for the date field being populated.
   * @param array $instance
   *   Metadata for this instance of the date field being populated.
   * @param array $values
   *   Array of date values to be fielded.
   *
   * @return array|null
   *   An array of date fields.
   */
  public function prepare($entity, array $field_info, array $instance, array $values) {
    $delta = 0;
    
    foreach ($values as $key=>$value) {
    	$return[LANGUAGE_NONE][$delta][$key] = $value;
  //  	$delta++;
    }
    if (!isset($return)) {
      $return = NULL;
    }
 //   print_r($return);
  //  throw new exception($values);
    return $return;
  }
}

