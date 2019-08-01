set markup html on spool on
set lines 200 pages 0 feedback off verify off
col owner for a30
col Nome_Acronimo for a8
col segment_name for a60

select 
(select value from v$parameter where name='db_name') as Nome_DB,
(select db_unique_name from v$database) as DB_Unique_name,
(select status from v$instance) as DB_Status,
((SELECT version FROM V$INSTANCE)||(select (case when UPPER(banner) like '%EXTREME%' then ' Extreme Edition' when UPPER(banner) like '%ENTERPRISE%' then ' Enterprise Edition' else ' Standard Edition' end) from v$version where rownum=1)) as Versione,
(SELECT platform_name  FROM V$database) as platform,
(SELECT log_mode  FROM V$database) as archive,
(select value from v$nls_parameters where parameter='NLS_CHARACTERSET') as Charset,
(select value from v$nls_parameters where parameter='NLS_NCHAR_CHARACTERSET') as NCharset,
(select value from v$parameter where name='db_block_size') as Blocksize,
(select value from v$parameter where name='cpu_count') as Cpu_count,
(select rtrim(to_char(value/1024/1024/1024, 'FM9G999G999D999', 'NLS_NUMERIC_CHARACTERS='',.'''),',') from v$parameter where name='sga_target')  as Sga_Target,
(select rtrim(to_char(value/1024/1024/1024, 'FM9G999G999D999', 'NLS_NUMERIC_CHARACTERS='',.'''),',') from v$parameter where name='pga_aggregate_target') as Pga_Target,
(select rtrim(to_char(value/1024/1024/1024, 'FM9G999G999D999', 'NLS_NUMERIC_CHARACTERS='',.'''),',') from v$parameter where name='memory_target') as Pga_Target,
(select rtrim(to_char(value/1024/1024/1024, 'FM9G999G999D999', 'NLS_NUMERIC_CHARACTERS='',.'''),',') from v$parameter where name='sga_max_size') as sga_max_size,
--(select round(sum(bytes/1024/1024/1024)) from dba_segments),
'N/A',
--((select round(sum(bytes/1024/1024/1024)) from v$datafile)+(select round(sum(bytes/1024/1024/1024)) from v$tempfile)+(select round(sum(bytes/1024/1024/1024)) from v$log)),
'N/A',
--((select round(sum(decode(autoextensible,'NO',bytes/1024/1024/1024,'YES',maxbytes/1024/1024/1024))) from v$datafile)+(select round(sum(bytes/1024/1024/1024)) from v$tempfile)+(select round(sum(bytes/1024/1024/1024)) from v$log)),
'N/A',
--(SELECT replace(replace(output,'Elapsed:',''),chr(32), '') FROM TABLE (DBMS_WORKLOAD_REPOSITORY.awr_report_text (:dbid, :inst_num, :bid, :eid, 0)) where rownum <20 and output like '%Elapsed: %'),
--(SELECT replace(replace(output,'DB Time:',''),chr(32), '') FROM TABLE (DBMS_WORKLOAD_REPOSITORY.awr_report_text (:dbid, :inst_num, :bid, :eid, 0)) where rownum <20 and output like '%DB Time: %'),
--:elapsed,:dbtime,(select :result from dual),
'N/A','N/A','N/A',
(select case when (select count(*) from v$datafile where name like '+%') > 0 then 'Y' else 'N' end as "ASM" from dual ),
case when ( select count(*) from V$DATAGUARD_CONFIG) > 1 then 'Y' else 'N' end  as "Dataguard"
from dual;
exit
