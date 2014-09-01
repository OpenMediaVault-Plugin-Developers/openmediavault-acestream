Ext.define("OMV.module.admin.service.acestream.Settings", { // Define a new class
extend: "OMV.workspace.form.Panel", // What is the base type of this class

rpcService: "AceStream", // Remote Procedure Call 
rpcGetMethod: "getSettings", // Remote Procedure Call 
rpcSetMethod: "setSettings", // Remote Procedure Call 

getFormItems: function() { // Generic function for this class that initializes the GUI
   return [{
      xtype: "fieldset", // Type of the item
      title: _("Ace Proxy settings"), // Text that is shown on the top edge of the fieldset
      fieldDefaults: {
         labelSeparator: ""
      },
      items: [{ // These items are inside the fieldset item defined above
         xtype: "checkbox", // Type of the item 
         name: "enable", // Individual name of the item
         fieldLabel: _("Enable"), // Text that is shown next to the checkbox. Keep this under 15 characters
         checked: false // Default value if no settings have been applied yet, Try to change this to true
         },
         {
         xtype: "textfield",
         name: "proxyhost",
         fieldLabel: "Bind Address",
		 vtype: "IPv4Net",
		 value      : "127.0.0.1",
		 plugins    : [{
                    ptype : "fieldinfo",
                    text  : "IP address to listen on. Use 0.0.0.0 for all host IPs."
                }]
         },
         {
         xtype: "numberfield",
         name: "proxyport",
         fieldLabel: "Port",
		 vtype: "port",
		 minValue      : 0,
         maxValue      : 65535,
		 value      : "8000",
		 plugins    : [{
                    ptype : "fieldinfo",
                    text  : "Port to listen on"
                }]
         },
         {
         xtype: "textfield",
         name: "streamhost",
         fieldLabel: "AceStream Engine IP",
		 vtype: "IPv4Net",
		 value      : "127.0.0.1",
		 plugins    : [{
                    ptype : "fieldinfo",
                    text  : "IP address of the AceStream Engine. Use 127.0.0.1 for local."
                }]
         },
         {
         xtype: "numberfield",
         name: "streamport",
		 minValue      : 0,
         maxValue      : 65535,
         fieldLabel: "AceStream Engine Port",
		 vtype: "port",
		 value      : "62062",
		 plugins    : [{
                    ptype : "fieldinfo",
                    text  : "Port of the AceStream Engine. Default 62062"
                }]
         }]
      }];
}
});

// Register the class that is defined above
OMV.WorkspaceManager.registerPanel({
id: "Settings", //Individual id
path: "/service/acestream", // Parent folder in the navigation view
text: _("Ace Stream"), // Text to show on the tab , Shown only if multiple form panels
position: 10, // Horizontal position of this tab. Use when you have multiple tabs
className: "OMV.module.admin.service.acestream.Settings" // Same class name as defined above
});