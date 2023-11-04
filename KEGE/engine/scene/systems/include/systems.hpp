//
//  systems.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/28/23.
//

#ifndef KEGE_INCLUDE_SYSTEMS_HPP
#define KEGE_INCLUDE_SYSTEMS_HPP

#include "KEGE.hpp"
#include "Core.hpp"
#include "Graphics.hpp"
#include "Message.hpp"
#include "event_enums.hpp"
#include "../../ecs/ECS.hpp"
#include "../../ecs/Entity.hpp"
#include "../../systems/components/components.hpp"
#include "../../systems/camera/Camera.hpp"
#include "../../systems/camera/CameraControl.hpp"
#include "../../systems/camera/CameraRotation.hpp"
#include "../../systems/camera/FloatingOrigin.hpp"
#include "../../systems/input/Input.hpp"
#include "../../systems/motion/Movement.hpp"
#include "../../systems/motion/MovementControl.hpp"
#include "../../systems/render/Render.hpp"
#include "../../systems/render/Hierarchy.hpp"
#include "../../systems/player/ActivePlayer.hpp"
#include "../../systems/motion/Accelerator.hpp"
#include "../../systems/motion/LinearMotion.hpp"
#include "../../systems/motion/AngularMotion.hpp"
#include "../../systems/spherical-terrain/Gravitation.hpp"
#include "../../systems/spherical-terrain/SphericalTerrain.hpp"
#include "../../systems/spherical-terrain/CelestialObjectUpdater.hpp"
#include "../../systems/spherical-terrain/LandscapeUpdater.hpp"

#endif /* KEGE_INCLUDE_SYSTEMS_HPP */
