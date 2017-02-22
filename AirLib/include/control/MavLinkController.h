// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_ros_MavLinkHelper_hpp
#define air_ros_MavLinkHelper_hpp

#include "common/CommonStructs.hpp"
#include "common/Common.hpp"
#include "common/VectorMath.hpp"
#include "vehicles/controllers/ControllerBase.hpp"
#include "vehicles/MultiRotor.hpp"
#include "DroneControlBase.hpp"
#include <queue>
#include <mutex>
#include <string>
#include <vector>

namespace msr { namespace airlib {


class MavLinkController : public ControllerBase
{
public:
    typedef msr::airlib::GeoPoint GeoPoint;
    typedef msr::airlib::VectorMath VectorMath;
    typedef msr::airlib::Vector3r Vector3r;
    typedef msr::airlib::Quaternionr Quaternionr;
    typedef common_utils::Utils Utils;
    typedef msr::airlib::real_T real_T;
    typedef msr::airlib::MultiRotor MultiRotor;

    struct HILConnectionInfo {
        /* Default values are requires so uninitialized instance doesn't have random values */
        std::string vehicle_name = "Pixhawk";
        bool use_serial = true; // false means use UDP instead

        //needed only if use_serial = false
        std::string ip_address = "127.0.0.1";
        int ip_port = 14560;

        //needed only if use_serial = true
        std::string serial_port = "*";
        int baud_rate = 115200;
    };

public:
    //required for pimpl
    MavLinkController();
    ~MavLinkController();

    void initialize(const HILConnectionInfo& connection_info, const MultiRotor* vehicle);
    HILConnectionInfo getHILConnectionInfo();
    int getRotorControlsCount();
    void connectToExternalSim();
    void connectToHIL();
    void connectToVideoServer();    
    bool connectToLogViewer();
    bool connectToQGC();
    void sendImage(unsigned char data[], uint32_t length, uint16_t width, uint16_t height);
    void getMocapPose(Vector3r& position, Quaternionr& orientation);
    void sendMocapPose(const Vector3r& position, const Quaternionr& orientation);
    void sendCollison(float normalX, float normalY, float normalZ);
    bool hasVideoRequest();
    void sendHILSensor(const Vector3r& acceleration, const Vector3r& gyro, const Vector3r& mag, float abs_pressure, float pressure_alt);
    void sendHILGps(const GeoPoint& geo_point, const Vector3r& velocity, float velocity_xy, float cog, float eph, float epv, int fix_type, unsigned int satellites_visible);
    void close();
    void setNormalMode();
    void setHILMode();
    static std::string findPixhawk();

    //*** Start: UpdatableState implementation ***//
    virtual void reset() override;
    virtual void update(real_T dt) override;
    virtual void start() override;
    virtual void stop() override;
    //*** End: UpdatableState implementation ***//
    virtual real_T getVertexControlSignal(unsigned int rotor_index) override;
    virtual void getStatusMessages(std::vector<std::string>& messages) override;
    virtual DroneControlBase* createOrGetDroneControl() override;

    struct impl;
    std::unique_ptr<impl> pimpl_;
};

}} //namespace
#endif