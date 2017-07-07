;; load
load "package://hrpsys_ros_bridge_tutorials/euslisp/chidori-interface.l"

(defun init ()
  (chidori-init)
  (send *ri* :start-auto-balancer)
  (send *ri* :start-st)
  )

(ros::load-ros-manifest "log_tools")

(defun energy-exp (&key (log_name "/home/leus/k-nguyen/energy-exp-20170707/exp0_13.5V_72V")
                        (rosbag_path "/home/leus/k-nguyen/energy-exp-20170707/")
                        (rosbag_name "exp0_13.5V_72V")
                        (stride-fwd 0.15) (step-time 1.2)
                        (orbit-type "cycloiddelay")
                        (double-support 0.35)
                        (distance 1))
   (ros::roseus "rosbag_command_client")
   (ros::wait-for-service "rosbag_command")
   ;; start hrpsys-log
   (send *ri* :set-log-maxlength (* 500 (* 60 10)))
   ;; start recording rosbag
   (let* ((req (instance log_tools::RosbagCommandRequest :init))
          (orbit (read-from-string (format nil "~A" orbit-type)))
          (_log_name (format nil "~A_sfwd~A_steptm~A_~A_dsr~A_dis~A" log_name stride-fwd step-time orbit-type double-support distance))
          (_bag_name (format nil "~A_sfwd~A_steptm~A_~A_dsr~A_dis~A_" rosbag_name stride-fwd step-time orbit-type double-support distance))
         )
    (send req :command "start")
    (send req :topics "/12V/power_info /80V/power_info")
    (send req :filename _bag_name)
    (send req :path rosbag_path)
    (setq *res* (ros::service-call "rosbag_command" req))
    (send *ri* :start-log)
    ;; Experiment
    (progn
      (send *ri* :set-gait-generator-param
            :default-step-time step-time
            :stride-parameter (float-vector stride-fwd 0.05 10.0 0.05) ;; front, side, rotation, back
            :default-orbit-type orbit ;; rectangle, cycloid, stair ...
            :default-double-support-ratio double-support
            ))
    (send *ri* :go-pos distance 0 0)
    ;; save hrpsys-log
    ;; (send *ri* :wait-interpolation-seq)
    ;; stop recording rosbag
    (send req :command "stop")
    (send req :topics "/12V/power_info /80V/power_info")
    ;; (send req :filename _bag_name)
    ;; (send req :path rosbag_path)
    (send *ri* :save-log _log_name)
    (setq *res* (ros::service-call "rosbag_command" req))
    )
)
