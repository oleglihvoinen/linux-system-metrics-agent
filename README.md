# Linux System Metrics Agent

A production-oriented **C/Linux observability agent** that reads host telemetry directly from Linux kernel interfaces and emits newline-delimited JSON for downstream collection.

![Architecture](https://raw.githubusercontent.com/oleglihvoinen/oleglihvoinen.github.io/main/assets/architecture/linux-system-metrics-agent.png)

## Executive summary

The agent collects CPU, memory, filesystem and network telemetry with minimal dependencies and a deliberately small runtime footprint. It is designed around a clean separation between **metric collection** and **transport**, so the same collector can feed Kafka, OpenTelemetry, Fluent Bit, journald or another observability/data platform without rewriting the sampling logic.

## Architecture

Linux kernel interfaces → C collector → JSONL telemetry → downstream collector → Kafka / observability platform / warehouse.

## Engineering design

- direct sampling from `/proc/stat`, `/proc/meminfo` and `/proc/net/dev`
- filesystem utilization via `statvfs()`
- CPU utilization derived from interval-based kernel counter deltas
- structured JSONL output for machine-readable downstream processing
- hardened `systemd` unit with restart policy and restricted privileges
- GCC build automation and GitHub Actions validation
- transport-independent collector boundary

## Build and run

```bash
make
./metrics-agent 5
```

The optional argument is the collection interval in seconds.

## Operational hardening

For enterprise deployment, the design can be extended with configuration files, TLS-authenticated transport, local buffering, backpressure handling, OpenTelemetry/Prometheus export, package management, structured diagnostics, service health endpoints and richer device-level metrics.

## Repository scope

This repository contains the collector, service definition and CI validation required to demonstrate the systems-engineering design. External telemetry backends are intentionally kept outside the collector boundary.

**Technologies:** C · Linux · POSIX · /proc · statvfs · systemd · JSON · GCC · GitHub Actions · observability
