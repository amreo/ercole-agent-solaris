set feedback off pages 0 serverout on verify off lines 1234
VARIABLE TUNING varchar2(100);
VARIABLE DIAGNOSTICS varchar2(100);
VARIABLE LABELSECURITY varchar2(100);
VARIABLE COMPRESSION varchar2(100);
VARIABLE ANALYTICS varchar2(100);
VARIABLE TESTING varchar2(100);
VARIABLE OLAP varchar2(100);
VARIABLE VAULT varchar2(100);
VARIABLE PARTITIONING varchar2(100);
VARIABLE RAC varchar2(100);
VARIABLE SPATIAL varchar2(100);
VARIABLE GATEWAY varchar2(100);
--
VARIABLE WEBLOGICSERVER  varchar2(100);
VARIABLE SECURE varchar2(100);
VARIABLE ONE varchar2(100);
VARIABLE PATCH1 varchar2(100);
VARIABLE PATCH2 varchar2(100);
VARIABLE PILLAR varchar2(100);
VARIABLE MULTITENANT varchar2(100);
VARIABLE HW varchar2(100);
VARIABLE GOLDEN varchar2(100);
VARIABLE EXADATA varchar2(100);
VARIABLE MASKING varchar2(100);
VARIABLE MEMORY varchar2(100);
VARIABLE CONFIGURATION varchar2(100);
VARIABLE SECURITY varchar2(100);
VARIABLE MANAGEMENT varchar2(100);
VARIABLE GUARD varchar2(100);
VARIABLE RAC2 varchar2(100);
begin
select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Automatic Maintenance - SQL Tuning Advisor','Automatic SQL Tuning Advisor','Real-Time SQL Monitoring','Real-Time SQL Monitoring','SQL Access Advisor','SQL Monitoring and Tuning pages','SQL Profile','SQL Tuning Advisor',
		'SQL Tuning Set (user)','Tuning Pack') 
		and detected_usages > 0 and dbid=(select dbid from v$database) )) > 0 
	then 'Y' else '' end into :TUNING from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('AWR Repository','Automatic Database Diagnostic Monitor','Automatic Workload Repository','Diagnostic Pack') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then 'Y' else '' end into :DIAGNOSTICS from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Label Security') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then 'Y' else '' end into :LABELSECURITY from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Data Guard') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then 'Y' else '' end into :COMPRESSION from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Data Mining') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then 'Y' else '' end into :ANALYTICS from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Database Replay: Workload Capture') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then 'Y' else '' end into :TESTING from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('OLAP - Analytic Workspaces','OLAP - Cubes') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then 'Y' else '' end into :OLAP from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Oracle Database Vault') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then 'Y' else '' end into :VAULT from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Partitioning (user)') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then 'Y' else '' end into :PARTITIONING  from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Real Application Clusters (RAC)') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then 'Y' else '' end into :RAC from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Transparent Gateway') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then 'Y' else '' end into :GATEWAY from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Spatial') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then 'Y' else '' end into :SPATIAL from dual;
end;
/


select 'WebLogic Server Management Pack Enterprise Edition:'||:WEBLOGICSERVER from dual;
select 'Tuning Pack:'||:TUNING from dual;
select 'Spatial and Graph:'||:SPATIAL from dual;
select 'Secure Backup:'||:SECURE from dual;
select 'Real Application Testing:'||:TESTING from dual;
select 'Real Application Clusters One Node:'||:ONE from dual;
select 'Real Application Clusters:'||:RAC from dual;
select 'RAC or RAC One Node:'||:RAC2 from dual;
select 'Provisioning and Patch Automation Pack for Database:'||:PATCH1 from dual;
select 'Provisioning and Patch Automation Pack:'||:PATCH2 from dual;
select 'Pillar Storage:'||:PILLAR from dual;
select 'Partitioning:'||:PARTITIONING from dual;
select 'OLAP:'||:OLAP from dual;
select 'Multitenant:'||:MULTITENANT from dual;
select 'Label Security:'||:LABELSECURITY from dual;
select 'HW:'||:HW from dual;
select 'GoldenGate:'||:GOLDEN from dual;
select 'Exadata:'||:EXADATA from dual;
select 'Diagnostics Pack:'||:DIAGNOSTICS from dual;
select 'Database Vault:'||:VAULT from dual;
select 'Database In-Memory:'||:MEMORY from dual;
select 'Database Gateway:'||:GATEWAY from dual;
select 'Data Masking Pack:'||:MASKING from dual;
select 'Configuration Management Pack for Oracle Database:'||:CONFIGURATION from dual;
select 'Change Management Pack:'||:MANAGEMENT from dual;
select 'Advanced Security:'||:SECURITY from dual;
select 'Advanced Compression:'||:COMPRESSION from dual;
select 'Advanced Analytics:'||:ANALYTICS from dual;
select 'Active Data Guard:'||:GUARD from dual;
