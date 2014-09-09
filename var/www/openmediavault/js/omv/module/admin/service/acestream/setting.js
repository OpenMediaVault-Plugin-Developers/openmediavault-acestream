Ext.define("OMV.module.admin.service.acestream.Settings", { // Define the class
	extend: "OMV.workspace.form.Panel", // Base type of the class

	rpcService: "AceStream", // set name for rpc service
	rpcGetMethod: "getSettings", // name for the function in the rpc that gets the settings
	rpcSetMethod: "setSettings", // name for the function in the rpc that saves the settings

	getFormItems: function() { //initialize the GUI
	   return [
		{
			xtype	:	"fieldset",
			title	:	_("Ace Proxy settings"),
			fieldDefaults:  {
								labelSeparator: ""
							},
			items	:	[
							{ 
								xtype	:	"checkbox",
								name	:	"enable",
								fieldLabel: _("Enable"),
								checked	:	false
							},
							{
								xtype	:	"textfield",
								name	:	"proxyhost",
								fieldLabel: "Bind Address",
								vtype	:	"IPv4Net",
								value   :	"127.0.0.1",
								plugins : 	[
												{
													ptype	: "fieldinfo",
													text	: "IP address to listen on. Use 0.0.0.0 for all host IPs."
												}
											]
							},
							{
								xtype	:	"numberfield",
								name	:	"proxyport",
								fieldLabel: "Port",
								vtype	:	"port",
								minValue: 	0,
								maxValue:	65535,
								value	:	"8000",
								plugins : 	[
												{
													ptype	:	"fieldinfo",
													text	:	"Port to listen on"
												}
											]
							}
						]
		  },
		  {
			xtype: "fieldset",
			title: _("VLC settings"),
			fieldDefaults: {
				labelSeparator: ""
			},
			items: [
					{
						xtype	:	"checkbox",
						name	:	"vlcuse", 
						fieldLabel:	_("Enable VLC"),
						checked	:	false,
						plugins	:	[
										{
											ptype	:	"fieldinfo",
											text	:	"Enable proxy to use vlc if installed."
										}
									]
					},
					{
						xtype	:	"textfield",
						name	:	"vlchost",
						fieldLabel: "VLC host",
						vtype	: 	"IPv4Net",
						value	:	"127.0.0.1",
						plugins :	[
										{
											ptype	:	"fieldinfo",
											text	:	"IP address of VLC. Use 0.0.0.0 for all host IPs."
										}
									]
					 },
					 {
						xtype	:	"numberfield",
						name	:	"vlcport",
						fieldLabel: "VLC Port",
						vtype	:	"port",
						minValue:	0,
						maxValue:	65535,
						value	:	"8000",
						plugins :	[
										{
											ptype	:	"fieldinfo",
											text	:	"Port vlc is listening on"
										}
									]
					 },
					 {
						xtype	:	"numberfield",
						name	:	"vlcoutport",
						fieldLabel: "VLC Output Port",
						vtype	: "port",
						minValue:	0,
						maxValue:	65535,
						value	:	"8082",
						plugins : 	[
										{
											ptype	:	"fieldinfo",
											text	:	"Port VLC outputs the stream on"
										}
									]
					 },
					 {
						xtype	:	"textfield",
						name	:	"vlcpass",
						fieldLabel: "VLC Telnet password",
						value	:	"admin",
						plugins: 	[
										{
											ptype	:	"fieldinfo",
											text	:	"VLC Telnet password"
										}
									]
					}
				   ]
			  }
		  ];
	}
});

// Register the class that is defined above
OMV.WorkspaceManager.registerPanel(
	{
		id:		"Settings", //Id of the class
		path:	"/service/acestream", // Parent folder in the navigation view
		text: 	_("Ace Stream"), // Text to show on the tab
		position: 	10,
		className:	"OMV.module.admin.service.acestream.Settings" // Same class name as defined above
	}
);