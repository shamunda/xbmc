/*
 *      Copyright (C) 2014-2016 Team Kodi
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "DriverPrimitive.h"
#include "JoystickTypes.h"

#include <string>
#include <vector>

namespace JOYSTICK
{
  /*!
   * \brief Button map interface to translate between the driver's raw
   *        button/hat/axis elements and physical joystick features.
   *
   * \sa IButtonMapper
   */
  class IButtonMap
  {
  public:
    virtual ~IButtonMap(void) { }

    /*!
     * \brief The add-on ID of the game controller associated with this button map
     *
     * The controller ID provided by the implementation serves as the context
     * for the feature names below.
     *
     * \return The ID of this button map's game controller add-on
     */
    virtual std::string ControllerID(void) const = 0;

    /*!
     * \brief The name of the peripheral associated with this button map
     *
     * \return The peripheral's name
     */
    virtual std::string DeviceName(void) const = 0;

    /*!
     * \brief Load the button map into memory
     *
     * \return True if button map is ready to start translating buttons, false otherwise
     */
    virtual bool Load(void) = 0;

    /*!
     * \brief Reset the button map to its defaults, or clear button map if no defaults
     */
    virtual void Reset(void) = 0;

    /*!
     * \brief Check if the button map is empty
     *
     * \return True if the button map is empty, false if it has features
     */
    virtual bool IsEmpty(void) const = 0;

    /*!
     * \brief Get the feature associated with a driver primitive
     *
     * Multiple primitives can be mapped to the same feature. For example,
     * analog sticks use one primitive for each direction.
     *
     * \param primitive    The driver primitive (a button, hat direction or semi-axis)
     * \param feature      The name of the resolved joystick feature, or
     *                     invalid if false is returned
     *
     * \return True if the driver primitive is associated with a feature, false otherwise
     */
    virtual bool GetFeature(
      const CDriverPrimitive& primitive,
      FeatureName& feature
    ) = 0;

    /*!
     * \brief Get the type of the feature for the given name
     *
     * \param feature      The feature to look up
     *
     * \return The feature's type
     */
    virtual FEATURE_TYPE GetFeatureType(const FeatureName& feature) = 0;

    /*!
     * \brief Get the driver primitive for a scalar feature
     *
     * When a feature can be represented by a single driver primitive, it is
     * called a scalar feature.
     *
     *   - This includes buttons and triggers, because they can be mapped to a
     *     single button/hat/semiaxis
     *
     *   - This does not include analog sticks, because they require two axes
     *     and four driver primitives (one for each semiaxis)
     *
     * \param feature        Must be a scalar feature (a feature that only
     *                       requires a single driver primitive)
     * \param primitive      The resolved driver primitive
     *
     * \return True if the feature resolved to a driver primitive, false if the
     *         feature didn't resolve or isn't a scalar feature
     */
    virtual bool GetScalar(
      const FeatureName& feature,
      CDriverPrimitive& primitive
    ) = 0;

    /*!
     * \brief Add or update a scalar feature
     *
     * \param feature        Must be a scalar feature
     * \param primitive      The feature's driver primitive
     *
     * \return True if the feature was updated, false if the feature is
     *         unchanged or failure occurs
     */
    virtual void AddScalar(
      const FeatureName& feature,
      const CDriverPrimitive& primitive
    ) = 0;

    /*!
     * \brief Get an analog stick direction from the button map
     *
     * \param      feature   Must be an analog stick or this will return false
     * \param      direction The direction whose primitive is to be retrieved
     * \param[out] primitive The primitive mapped to the specified direction
     *
     * \return True if the feature and direction resolved to a driver primitive
     */
    virtual bool GetAnalogStick(
      const FeatureName& feature,
      ANALOG_STICK_DIRECTION direction,
      CDriverPrimitive& primitive
    ) = 0;

    /*!
     * \brief Add or update an analog stick direction
     *
     * \param feature   Must be an analog stick or this will return false
     * \param direction The direction being mapped
     * \param primitive The driver primitive for the specified analog stick and direction
     *
     * \return True if the analog stick was updated, false otherwise
     */
    virtual void AddAnalogStick(
      const FeatureName& feature,
      ANALOG_STICK_DIRECTION direction,
      const CDriverPrimitive& primitive
    ) = 0;

    /*!
     * \brief Get an accelerometer from the button map
     *
     * \param feature       Must be an accelerometer or this will return false
     * \param positiveX     The semiaxis mapped to the positive X direction (possibly unknown)
     * \param positiveY     The semiaxis mapped to the positive Y direction (possibly unknown)
     * \param positiveZ     The semiaxis mapped to the positive Z direction (possibly unknown)
     *
     * \return True if the feature resolved to an accelerometer with at least 1 known axis
     */
    virtual bool GetAccelerometer(
      const FeatureName& feature,
      CDriverPrimitive& positiveX,
      CDriverPrimitive& positiveY,
      CDriverPrimitive& positiveZ
    ) = 0;

    /*!
     * \brief Get or update an accelerometer
     *
     * \param feature       Must be an accelerometer or this will return false
     * \param positiveX     The semiaxis corresponding to the positive X direction
     * \param positiveY     The semiaxis corresponding to the positive Y direction
     * \param positiveZ     The semiaxis corresponding to the positive Z direction
     *
     * The driver primitives must be mapped to a semiaxis or this function will fail.
     *
     * \return True if the accelerometer was updated, false if unchanged or failure occurred
     */
    virtual void AddAccelerometer(
      const FeatureName& feature,
      const CDriverPrimitive& positiveX,
      const CDriverPrimitive& positiveY,
      const CDriverPrimitive& positiveZ
    ) = 0;

    /*!
     * \brief Set a list of driver primitives to be ignored
     *
     * This is necessary to prevent features from interfering with the button
     * mapping process. This includes accelerometers, as well as PS4 triggers
     * which send both a button press and an analog value.
     *
     * \param primitives      The driver primitives to be ignored
     */
    virtual void SetIgnoredPrimitives(const std::vector<CDriverPrimitive>& primitives) = 0;

    /*!
     * \brief Check if a primitive is in the list of primitives to be ignored
     *
     * \param primitive      The primitive to check
     *
     * \return True if the primitive should be ignored in the mapping process
     */
    virtual bool IsIgnored(const CDriverPrimitive& primitive) = 0;

    /*!
     * \brief Save the button map
     */
    virtual void SaveButtonMap() = 0;

    /*!
    * \brief Revert changes to the button map since the last time it was loaded
    *        or commited to disk
    */
    virtual void RevertButtonMap() = 0;
  };
}
