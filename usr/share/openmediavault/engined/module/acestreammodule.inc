<?php

/**
 * Copyright (C) 2013-2014 OpenMediaVault Plugin Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

require_once("openmediavault/module.inc");
require_once("openmediavault/config.inc");
require_once("openmediavault/error.inc");
require_once("openmediavault/initscript.inc");
  
class OMVModuleAceStream extends OMVModuleServiceAbstract implements
    OMVINotifyListener,
    OMVIModuleServiceStatus
{
    public function getName()
    {
        return "AceStream";
    }

    private function getPluginName()
    {
        return strtolower($this->getName());
    }

    private function getEventMessagePath()
    {
        return sprintf("org.openmediavault.service.%s", $this->getPluginName());
    }

    private function getXPath()
    {
        return sprintf("//services/%s", $this->getPluginName());
    }
  
    public function applyConfig()
    {
        $cmd = sprintf("omv-mkconf %s 2>&1", $this->getPluginName());
        if(0 !== $this->exec($cmd, $output)) {
            throw new OMVException(OMVErrorMsg::E_EXEC_FAILED, $cmd, implode("\n", $output));
        }
    }
  
    public function getStatus()
    {
        global $xmlConfig;
  
        // Get the configuration object.
        $object = $xmlConfig->get($this->getXPath());

        if (is_null($object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $this->getXPath()
            );
        }

          // If pid exists assume server is running
          $running = false;
          if (file_exists("/var/run/acestream.pid")) {
              $running = true;
          }

        return array(
            "name"    => $this->getName(),
            "title"   => gettext("Ace Stream"),
            "enabled" => boolval($object['enable']),
            "running" => $running
        );
    }

    public function bindListeners(OMVNotifyDispatcher $dispatcher)
    {
        $dispatcher->addListener(
            OMV_NOTIFY_MODIFY,
            $this->getEventMessagePath(),
            array($this, "setDirty")
        );
    }
}
  
  // Register the Module
  $moduleMgr = &OMVModuleMgr::getInstance();
  $moduleMgr->registerModule(new OMVModuleAceStream());
