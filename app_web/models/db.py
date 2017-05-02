# -*- coding: utf-8 -*-

from gluon.contrib.appconfig import AppConfig
myconf = AppConfig(reload=True)

db = DAL(myconf.get('db.uri'),
         pool_size=myconf.get('db.pool_size'),
         migrate_enabled=myconf.get('db.migrate'),
         check_reserved=['all'])

from gluon.tools import Auth, Service, PluginManager

auth = Auth(db, host_names=myconf.get('host.names'))
service = Service()
plugins = PluginManager()

# -------------------------------------------------------------------------
# create all tables needed by auth if not custom tables
# -------------------------------------------------------------------------
auth.define_tables(username=False, signature=False)


# -------------------------------------------------------------------------
# Define your tables below (or better in another model file) for example
#
# >>> db.define_table('mytable', Field('myfield', 'string'))
#
# Fields can be 'string','text','password','integer','double','boolean'
#       'date','time','datetime','blob','upload', 'reference TABLENAME'
# There is an implicit 'id integer autoincrement' field
# Consult manual for more options, validators, etc.
#
# More API examples for controllers:
#
# >>> db.mytable.insert(myfield='value')
# >>> rows = db(db.mytable.myfield == 'value').select(db.mytable.ALL)
# >>> for row in rows: print row.id, row.myfield
# -------------------------------------------------------------------------

import datetime
hoje = datetime.date.today()

db.define_table('movimentos', 
                Field('movimento', 'integer'),
                Field('tipo_movimento', 'integer'),
                Field('dia', 'date'))
