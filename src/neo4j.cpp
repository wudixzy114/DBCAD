#include "common.hxx"
#include "neo4j.hxx"
#include <format>

Neo4jPart::Neo4jPart(const char* host, int port_bolt, const char* un, const char* pw,
                     const std::string& pn) : partname(pn)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    mg_session_params* params = mg_session_params_make();
    mg_session_params_set_host(params, host);
    mg_session_params_set_port(params, (uint16_t)port_bolt);
    //mg_session_params_set_username(params, un);
    //mg_session_params_set_password(params, pw);
    mg_session_params_set_sslmode(params, MG_SSLMODE_DISABLE);
    //若要启用SSL功能，请换成这句：mg_session_params_set_sslmode(params, MG_SSLMODE_REQUIRE);
    int status = mg_connect(params, &session);
    mg_session_params_destroy(params);
    if (status < 0)
    {
        mg_session_destroy(session);
        myerror(std::format("Neo4j数据库连接失败，错误信息如下：{}", mg_session_error(session)));
        throw std::runtime_error("Neo4j Connect Failed");
    }
}

void Neo4jPart::execute_bolt(const char* statement, const mg_map* parameters) const
{
    if (!session)
    {
        myerror("尝试在无效的会话上执行 Bolt 查询。");
        return;
    }

    // 修复逻辑：处理传入 NULL 的情况，mg_session_run 不允许 parameters 为空
    mg_map* temp_empty_params = nullptr;
    const mg_map* safe_parameters = parameters;

    if (safe_parameters == nullptr)
    {
        temp_empty_params = mg_map_make_empty(0);
        safe_parameters = temp_empty_params;
    }

    int status = mg_session_run(session, statement, safe_parameters, NULL, NULL, NULL);

    // 如果创建了临时 Map，必须在使用后销毁以防止内存泄漏
    if (temp_empty_params)
    {
        mg_map_destroy(temp_empty_params);
    }

    // 检查执行结果
    if (status < 0)
    {
        const char* err = mg_session_error(session);
        myerror(std::format("执行Cypher语句失败: {}", err ? err : "Unknown error"));
    }

    if (mg_session_pull(session, NULL) < 0)
    {
        const char* err = mg_session_error(session);
        myerror(std::format("拉取Cypher语句执行结果失败: {}", err ? err : "Unknown error"));
    }
}


void Neo4jPart::discard_all_results() const
{
    mg_result* result;
    int status;
    while (1)
    {
        status = mg_session_fetch(session, &result);
        if (status == 0)
        {
            break;
        }
        else if (status != 1)
        {
            myerror(std::format("mg_session_fetch失败，错误信息如下：{}", mg_session_error(session)));
        }
    }
}
