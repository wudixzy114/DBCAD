#include "access_restore_dispatch.hxx"

#include "access_switch_registry.hxx"
#include "common.hxx"

#include <acis/include/allcurve.hxx>
#include <acis/include/alltop.hxx>
#include <acis/include/allsurf.hxx>
#include <acis/include/exct_spl.hxx>
#include <acis/include/exp_par.hxx>
#include <acis/include/int_int.hxx>
#include <acis/include/pcurve.hxx>

namespace AccessUtils::Restore::Dispatch
{
    void ApplyNeo4jEdge(
        const std::string& relTypename,
        int64_t relStartNodeId,
        int64_t relEndNodeId,
        std::unordered_map<int64_t, void*>& id2ptr)
    {
        switch (AccessUtils::Restore::Neo4jEdge_str2enum.at(relTypename))
        {
        case AccessUtils::Restore::Neo4jEdge::body_lump_ptr:
        {
            class BODY* body = (class BODY*)id2ptr.at(relStartNodeId);
            class LUMP* lump = (class LUMP*)id2ptr.at(relEndNodeId);
            body->set_lump(lump);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::body_wire_ptr:
        {
            class BODY* body = (class BODY*)id2ptr.at(relStartNodeId);
            class WIRE* wire = (class WIRE*)id2ptr.at(relEndNodeId);
            body->set_wire(wire);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::body_transform_ptr:
        {
            class BODY* body = (class BODY*)id2ptr.at(relStartNodeId);
            class TRANSFORM* transform = (class TRANSFORM*)id2ptr.at(relEndNodeId);
            body->set_transform(transform);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::lump_next_ptr:
        {
            class LUMP* lump = (class LUMP*)id2ptr.at(relStartNodeId);
            class LUMP* next = (class LUMP*)id2ptr.at(relEndNodeId);
            lump->set_next(next);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::lump_shell_ptr:
        {
            class LUMP* lump = (class LUMP*)id2ptr.at(relStartNodeId);
            class SHELL* shell = (class SHELL*)id2ptr.at(relEndNodeId);
            lump->set_shell(shell);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::lump_body_ptr:
        {
            class LUMP* lump = (class LUMP*)id2ptr.at(relStartNodeId);
            class BODY* body = (class BODY*)id2ptr.at(relEndNodeId);
            lump->set_body(body);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::shell_next_ptr:
        {
            class SHELL* shell = (class SHELL*)id2ptr.at(relStartNodeId);
            class SHELL* next = (class SHELL*)id2ptr.at(relEndNodeId);
            shell->set_next(next);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::shell_subshell_ptr:
        {
            class SHELL* shell = (class SHELL*)id2ptr.at(relStartNodeId);
            class SUBSHELL* subshell = (class SUBSHELL*)id2ptr.at(relEndNodeId);
            shell->set_subshell(subshell);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::shell_face_ptr:
        {
            class SHELL* shell = (class SHELL*)id2ptr.at(relStartNodeId);
            class FACE* face = (class FACE*)id2ptr.at(relEndNodeId);
            shell->set_face(face);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::shell_wire_ptr:
        {
            class SHELL* shell = (class SHELL*)id2ptr.at(relStartNodeId);
            class WIRE* wire = (class WIRE*)id2ptr.at(relEndNodeId);
            shell->set_wire(wire);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::shell_lump_ptr:
        {
            class SHELL* shell = (class SHELL*)id2ptr.at(relStartNodeId);
            class LUMP* lump = (class LUMP*)id2ptr.at(relEndNodeId);
            shell->set_lump(lump);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::subshell_parent_ptr:
        {
            class SUBSHELL* subshell = (class SUBSHELL*)id2ptr.at(relStartNodeId);
            class SUBSHELL* parent = (class SUBSHELL*)id2ptr.at(relEndNodeId);
            subshell->set_parent(parent);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::subshell_sibling_ptr:
        {
            class SUBSHELL* subshell = (class SUBSHELL*)id2ptr.at(relStartNodeId);
            class SUBSHELL* sibling = (class SUBSHELL*)id2ptr.at(relEndNodeId);
            subshell->set_sibling(sibling);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::subshell_child_ptr:
        {
            class SUBSHELL* subshell = (class SUBSHELL*)id2ptr.at(relStartNodeId);
            class SUBSHELL* child = (class SUBSHELL*)id2ptr.at(relEndNodeId);
            subshell->set_child(child);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::subshell_face_ptr:
        {
            class SUBSHELL* subshell = (class SUBSHELL*)id2ptr.at(relStartNodeId);
            class FACE* face = (class FACE*)id2ptr.at(relEndNodeId);
            subshell->set_face(face);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::subshell_wire_ptr:
        {
            class SUBSHELL* subshell = (class SUBSHELL*)id2ptr.at(relStartNodeId);
            class WIRE* wire = (class WIRE*)id2ptr.at(relEndNodeId);
            subshell->set_wire(wire);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::wire_next_ptr:
        {
            class WIRE* wire = (class WIRE*)id2ptr.at(relStartNodeId);
            class WIRE* next = (class WIRE*)id2ptr.at(relEndNodeId);
            wire->set_next(next);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::wire_coedge_ptr:
        {
            class WIRE* wire = (class WIRE*)id2ptr.at(relStartNodeId);
            class COEDGE* coedge = (class COEDGE*)id2ptr.at(relEndNodeId);
            wire->set_coedge(coedge);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::wire_owner_ptr:
        {
            class WIRE* wire = (class WIRE*)id2ptr.at(relStartNodeId);
            class ENTITY* owner = (class ENTITY*)id2ptr.at(relEndNodeId);
            wire->set_owner(owner);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::wire_subshell_ptr:
        {
            class WIRE* wire = (class WIRE*)id2ptr.at(relStartNodeId);
            class SUBSHELL* subshell = (class SUBSHELL*)id2ptr.at(relEndNodeId);
            wire->set_subshell(subshell);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::face_next_ptr:
        {
            class FACE* face = (class FACE*)id2ptr.at(relStartNodeId);
            class FACE* next = (class FACE*)id2ptr.at(relEndNodeId);
            face->set_next(next);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::face_loop_ptr:
        {
            class FACE* face = (class FACE*)id2ptr.at(relStartNodeId);
            class LOOP* loop = (class LOOP*)id2ptr.at(relEndNodeId);
            face->set_loop(loop);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::face_shell_ptr:
        {
            class FACE* face = (class FACE*)id2ptr.at(relStartNodeId);
            class SHELL* shell = (class SHELL*)id2ptr.at(relEndNodeId);
            face->set_shell(shell);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::face_subshell_ptr:
        {
            class FACE* face = (class FACE*)id2ptr.at(relStartNodeId);
            class SUBSHELL* subshell = (class SUBSHELL*)id2ptr.at(relEndNodeId);
            face->set_subshell(subshell);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::face_geometry_ptr:
        {
            class FACE* face = (class FACE*)id2ptr.at(relStartNodeId);
            class SURFACE* geometry = (class SURFACE*)id2ptr.at(relEndNodeId);
            face->set_geometry(geometry);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::loop_next_ptr:
        {
            class LOOP* loop = (class LOOP*)id2ptr.at(relStartNodeId);
            class LOOP* next = (class LOOP*)id2ptr.at(relEndNodeId);
            loop->set_next(next);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::loop_start_ptr:
        {
            class LOOP* loop = (class LOOP*)id2ptr.at(relStartNodeId);
            class COEDGE* start = (class COEDGE*)id2ptr.at(relEndNodeId);
            loop->set_start(start);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::loop_face_ptr:
        {
            class LOOP* loop = (class LOOP*)id2ptr.at(relStartNodeId);
            class FACE* face = (class FACE*)id2ptr.at(relEndNodeId);
            loop->set_face(face);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::coedge_next_ptr:
        {
            class COEDGE* coedge = (class COEDGE*)id2ptr.at(relStartNodeId);
            class COEDGE* next = (class COEDGE*)id2ptr.at(relEndNodeId);
            coedge->set_next(next);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::coedge_previous_ptr:
        {
            class COEDGE* coedge = (class COEDGE*)id2ptr.at(relStartNodeId);
            class COEDGE* previous = (class COEDGE*)id2ptr.at(relEndNodeId);
            coedge->set_previous(previous);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::coedge_partner_ptr:
        {
            class COEDGE* coedge = (class COEDGE*)id2ptr.at(relStartNodeId);
            class COEDGE* partner = (class COEDGE*)id2ptr.at(relEndNodeId);
            coedge->set_partner(partner);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::coedge_edge_ptr:
        {
            class COEDGE* coedge = (class COEDGE*)id2ptr.at(relStartNodeId);
            class EDGE* edge = (class EDGE*)id2ptr.at(relEndNodeId);
            coedge->set_edge(edge);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::coedge_owner_ptr:
        {
            class COEDGE* coedge = (class COEDGE*)id2ptr.at(relStartNodeId);
            class ENTITY* owner = (class ENTITY*)id2ptr.at(relEndNodeId);
            coedge->set_owner(owner);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::coedge_geometry_ptr:
        {
            class COEDGE* coedge = (class COEDGE*)id2ptr.at(relStartNodeId);
            class PCURVE* geometry = (class PCURVE*)id2ptr.at(relEndNodeId);
            coedge->set_geometry(geometry);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::edge_start_ptr:
        {
            class EDGE* edge = (class EDGE*)id2ptr.at(relStartNodeId);
            class VERTEX* start = (class VERTEX*)id2ptr.at(relEndNodeId);
            edge->set_start(start);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::edge_end_ptr:
        {
            class EDGE* edge = (class EDGE*)id2ptr.at(relStartNodeId);
            class VERTEX* end = (class VERTEX*)id2ptr.at(relEndNodeId);
            edge->set_end(end);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::edge_coedge_ptr:
        {
            class EDGE* edge = (class EDGE*)id2ptr.at(relStartNodeId);
            class COEDGE* coedge = (class COEDGE*)id2ptr.at(relEndNodeId);
            edge->set_coedge(coedge);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::edge_geometry_ptr:
        {
            class EDGE* edge = (class EDGE*)id2ptr.at(relStartNodeId);
            class CURVE* geometry = (class CURVE*)id2ptr.at(relEndNodeId);
            edge->set_geometry(geometry);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::vertex_edge_ptr:
        {
            class VERTEX* vertex = (class VERTEX*)id2ptr.at(relStartNodeId);
            class EDGE* edge = (class EDGE*)id2ptr.at(relEndNodeId);
            vertex->set_edge(edge);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::vertex_geometry_ptr:
        {
            class VERTEX* vertex = (class VERTEX*)id2ptr.at(relStartNodeId);
            class APOINT* geometry = (class APOINT*)id2ptr.at(relEndNodeId);
            vertex->set_geometry(geometry);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::pcurve_ref_curve_ptr:
        {
            class PCURVE* pcurve = (class PCURVE*)id2ptr.at(relStartNodeId);
            class CURVE* refCurve = (class CURVE*)id2ptr.at(relEndNodeId);
            pcurve->set_def(refCurve, pcurve->index(), 0, pcurve->offset());
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::pcurve_fit_ptr:
        {
            class PCURVE* p = (class PCURVE*)id2ptr.at(relStartNodeId);
            par_cur* fit = (par_cur*)id2ptr.at(relEndNodeId);
            p->set_fit(fit);
            fit->add_ref();
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::spline_surface_spl_ptr:
        {
            class SPLINE* splineSurface = (class SPLINE*)id2ptr.at(relStartNodeId);
            spl_sur* spl = (spl_sur*)id2ptr.at(relEndNodeId);
            splineSurface->gme_set_spl(spl);
            spl->add_ref();
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::intcurve_curve_fit_ptr:
        {
            class INTCURVE* intcurveCurve = (class INTCURVE*)id2ptr.at(relStartNodeId);
            int_cur* fit = (int_cur*)id2ptr.at(relEndNodeId);
            intcurveCurve->gme_set_fit(fit);
            fit->add_ref();
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::int_cur_surf1_spl_ptr:
        {
            int_cur* ic = (int_cur*)id2ptr.at(relStartNodeId);
            spl_sur* surf1Spl = (spl_sur*)id2ptr.at(relEndNodeId);
            ic->set_surf1_spl(surf1Spl);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::int_cur_surf2_spl_ptr:
        {
            int_cur* ic = (int_cur*)id2ptr.at(relStartNodeId);
            spl_sur* surf2Spl = (spl_sur*)id2ptr.at(relEndNodeId);
            ic->set_surf2_spl(surf2Spl);
        }
        break;
        case AccessUtils::Restore::Neo4jEdge::par_cur_surf_spl_ptr:
        {
            par_cur* pc = (par_cur*)id2ptr.at(relStartNodeId);
            spl_sur* surfSpl = (spl_sur*)id2ptr.at(relEndNodeId);
            ((exp_par_cur*)pc)->gme_set_surf_spl(surfSpl);
        }
        break;
        default:
        {
            myerror("不支持的neo4j边类型。");
        }
        break;
        }
    }
}
