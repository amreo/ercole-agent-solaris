set linesize 132 verify off
set pagesize 0 feedback off
set markup html on spool on

--column "TOTAL ALLOC (MB)" format 9,999,990
--column "TOTAL PHYS ALLOC (MB)" format 9,999,990
--column "USED (MB)" format  9,999,990
--column "FREE (MB)" format 9,999,990
column "% USED" format 990.00
column "Nome_Acronimo" for a8
column "DB_Name" for a10

select 
	   (select host_name from v$instance) as Hostname,
           (select value from v$parameter where name='db_name') as Nome_DB,
           (select db_unique_name from v$database) as DB_Unique_name,
	   a.tablespace_name,
       a.bytes_alloc/(1024*1024) "TOTAL ALLOC (MB)",
       a.physical_bytes/(1024*1024) "TOTAL PHYS ALLOC (MB)",
       nvl(b.tot_used,0)/(1024*1024) "USED (MB)",
       (nvl(b.tot_used,0)/a.bytes_alloc)*100  "% USED",
	   c.status
from ( select tablespace_name,
       sum(bytes) physical_bytes,
       sum(decode(autoextensible,'NO',bytes,'YES',maxbytes)) bytes_alloc
       from dba_data_files
       group by tablespace_name ) a,
     ( select tablespace_name, sum(bytes) tot_used
       from dba_segments
       group by tablespace_name ) b,
	 ( select tablespace_name,status from dba_tablespaces ) c
where a.tablespace_name = b.tablespace_name (+)
and a.tablespace_name = c.tablespace_name (+)
--and   (nvl(b.tot_used,0)/a.bytes_alloc)*100 > 10
--and   a.tablespace_name not in (select distinct tablespace_name from dba_temp_files)
--and   a.tablespace_name not like 'UNDO%'
order by 1;
--order by 5
exit
