#include "VideoRecognitionTask.h"

#include "Task/Experiment/CombatRecordRecognitionTask.h"
#include "Utils/Logger.hpp"

#include <filesystem>

asst::VideoRecognitionTask::VideoRecognitionTask(const AsstCallback& callback, Assistant* inst)
    : InterfaceTask(callback, inst, TaskType)
{}

bool asst::VideoRecognitionTask::set_params(const json::value& params)
{
    LogTraceFunction;

    if (m_running) {
        return false;
    }

    auto filename_opt = params.find<std::string>("filename");
    if (!filename_opt) {
        Log.error("No filename");
        return false;
    }
    std::string stage_name = params.get("stage", "");

    auto export_task_ptr = std::make_shared<CombatRecordRecognitionTask>(m_callback, m_inst, TaskType);
    export_task_ptr->set_retry_times(0);
    bool ret = export_task_ptr->set_video_path(*filename_opt);
    ret &= export_task_ptr->set_stage_name(stage_name);
    if (!ret) {
        return false;
    }

    export_task_ptr->set_retry_times(0);
    m_subtasks.emplace_back(std::move(export_task_ptr));

    return true;
}