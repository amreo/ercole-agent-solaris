set linesize 132  verify off
set pagesize 0 feedback off
set markup html on spool on

column "Nome_Acronimo" for a8
column "DB_Name" for a10

select 
	   (select host_name from v$instance) as Hostname,
	   'ND',
	   (select value from v$parameter where name='db_name') as Nome_DB,
	   (select db_unique_name from v$database) as DB_Unique_name,
           ( select version from v$instance) as Version,
	   PATCH_ID,ACTION,DESCRIPTION,to_char(action_time,'DD-MON-YYYY') from  registry$sqlpatch order by action_time;
exit
