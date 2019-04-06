# Standard REST Model

GET /tags : Liste aller Tags
GET /tags/:id : Informationen des einzelnen Tags
POST /tags/:id/alarm : Alarm auslösen

# More Mongoose-Friendly Way:

GET /tags
POST /alarm?id=...

