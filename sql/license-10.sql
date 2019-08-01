set feedback off pages 0 serverout on verify off lines 1234
VARIABLE TUNING number;
VARIABLE DIAGNOSTICS number;
VARIABLE LABELSECURITY number;
VARIABLE COMPRESSION number;
VARIABLE ANALYTICS number;
VARIABLE TESTING number;
VARIABLE OLAP number;
VARIABLE VAULT number;
VARIABLE PARTITIONING number;
VARIABLE RAC number;
VARIABLE SPATIAL number;
VARIABLE GATEWAY number;
--
VARIABLE WEBLOGICSERVER  number;
VARIABLE SECURE number;
VARIABLE ONE number;
VARIABLE PATCH1 number;
VARIABLE PATCH2 number;
VARIABLE PILLAR number;
VARIABLE MULTITENANT number;
VARIABLE HW number;
VARIABLE GOLDEN number;
VARIABLE EXADATA number;
VARIABLE MASKING number;
VARIABLE MEMORY number;
VARIABLE CONFIGURATION number;
VARIABLE SECURITY number;
VARIABLE MANAGEMENT number;
VARIABLE GUARD number;
VARIABLE RAC2 number;
begin
select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Automatic Maintenance - SQL Tuning Advisor','Automatic SQL Tuning Advisor','Real-Time SQL Monitoring','Real-Time SQL Monitoring','SQL Access Advisor','SQL Monitoring and Tuning pages','SQL Profile','SQL Tuning Advisor',
		'SQL Tuning Set (user)','Tuning Pack') 
		and detected_usages > 0 and dbid=(select dbid from v$database) )) > 0 
	then &&1*&&2 else null end into :TUNING from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('AWR Repository','Automatic Database Diagnostic Monitor','Automatic Workload Repository','Diagnostic Pack') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then &&1*&&2 else null end into :DIAGNOSTICS from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Label Security') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then &&1*&&2 else null end into :LABELSECURITY from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Data Guard') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then &&1*&&2 else null end into :COMPRESSION from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Data Mining') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then &&1*&&2 else null end into :ANALYTICS from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Database Replay: Workload Capture') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then &&1*&&2 else null end into :TESTING from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('OLAP - Analytic Workspaces','OLAP - Cubes') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then &&1*&&2 else null end into :OLAP from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Oracle Database Vault') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then &&1*&&2 else null end into :VAULT from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Partitioning (user)') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then &&1*&&2 else null end into :PARTITIONING  from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Real Application Clusters (RAC)') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then &&1*&&2 else null end into :RAC from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Transparent Gateway') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then &&1*&&2 else null end into :GATEWAY from dual;

select case when 
	(select count(*) 
		from dba_feature_usage_statistics 
		where (name in('Spatial') 
		and detected_usages > 0 and dbid=(select dbid from v$database))) > 0 
	then &&1*&&2 else null end into :SPATIAL from dual;
end;
/

select 'WebLogicServer ManagementPack EnterpriseEdition;'||:WEBLOGICSERVER||';' from dual; 
select 'Tuning Pack;'||:TUNING||';' from dual; 
select 'Spatial and Graph;'||:SPATIAL||';' from dual; 
select 'Secure Backup;'||:SECURE ||';' from dual; 
select 'Real Application Testing;'||:TESTING ||';' from dual; 
select 'Real Application Clusters;'||:ONE ||';' from dual; 
select 'Real Application Clusters One Node;'||:RAC ||';' from dual; 
select 'RAC or RAC One Node;'||:RAC2||';' from dual; 
select 'Provisioning and Patch Automation Pack;'||:PATCH1 ||';' from dual; 
select 'Provisioning and Patch Automation Pack for Database;'||:PATCH2 ||';' from dual; 
select 'Pillar Storage;'||:PILLAR ||';' from dual; 
select 'Partitioning;'||:PARTITIONING ||';' from dual; 
select 'OLAP;'||:OLAP ||';' from dual; 
select 'Multitenant;'||:MULTITENANT ||';' from dual; 
select 'Label Security;'||:LABELSECURITY ||';' from dual; 
select 'HW;'||:HW ||';' from dual; 
select 'Golden Gate;'||:GOLDEN ||';' from dual; 
select 'Exadata;'||:EXADATA ||';' from dual; 
select 'Diagnostics Pack;'||:DIAGNOSTICS ||';' from dual; 
select 'Database Vault;'||:MASKING ||';' from dual; 
select 'Database In-Memory;'||:VAULT ||';' from dual; 
select 'Database Gateway;'||:MEMORY ||';' from dual; 
select 'Data Masking Pack;'||:GATEWAY ||';' from dual; 
select 'Configuration Management Pack for Oracle Database;'||:CONFIGURATION ||';' from dual; 
select 'Change Management Pack;'||:MANAGEMENT||';' from dual; 
select 'Advanced Security;'||:SECURITY ||';' from dual; 
select 'Advanced Compression;'||:COMPRESSION ||';' from dual; 
select 'Advanced Analytics;'||:ANALYTICS||';' from dual; 
select 'Active Data Guard;'||:GUARD||';' from dual; 

spool off
exit
